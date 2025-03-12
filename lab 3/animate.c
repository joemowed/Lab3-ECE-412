/*
 * animate.c
 *
 * Created: 3/8/2025 8:57:12 AM
 *  Author: Joe Maloney
 */
#include "animate.h"

#include <stdbool.h>
#include <stdint-gcc.h>

#include "LCD.h"
#include "delay.h"
#include "font.h"
#include "regAPI.h"

// 2d array, pixelBuffer[0:7] holds the
// rows of pixels to be sent to drawFrame
static uint8_t pixelBuffer[8][ANIMATE_BUFFER_SIZE];

// total number of valid bytes in each row of pixelBuffer
// also used as an index when loading chars into pixelBuffer
static uint8_t drawLength = 0;

// global used to track the number of bytes used by the 1-pixel gap between
// each byte
static uint8_t gapDrawLength = 0;

// string to be sent to the LCD if the given string would not fit in the
// pixelBuffer
char errString[] = "Error: String too Large";

// carryIn: value to be shifted into LSB of value
// value: shifts this value 1 pos to the left and writes carryIn to LSB
// return: The MSB of the initial value (carry-out)
bool LSLCarry(uint8_t *value, bool carryIn) {
  // Font is only 5 pixels (bits) wide, so we take
  // the carry as bit #4 (the 5th bit of the byte).
  bool ret = READ_BIT((*value), 4);
  // shift the value 1 space to the left
  *value <<= 1;
  // load the carry-in bit into the LSB of the current value
  *value |= carryIn;
  // return the carry-out bit
  return ret;
}

// helper function for animateShift().  Shifts a single row of pixelBuffer.
void animateShiftRow(uint8_t *rowStart) {
  // MSB of the start of row
  bool MSBCarry = READ_BIT(rowStart[0], 7);
  // start at the last char of the array
  int8_t i = drawLength - 1;
  // shift the last char in the array, with the MSB of the first byte in the
  // array as the carry-in bit
  bool carry = LSLCarry(&rowStart[i], MSBCarry);
  i--;
  // shift the rest of the chars in the array, chaining the carry from
  // each shift into the next
  for (; i >= 0; i--) {
    carry = LSLCarry(&rowStart[i], carry);
  }
}

// shifts all bits in the buffer 1 place to the left, rotating the MSB to the
// LSB
void animateShift() {
  // shift each row of the pixelBuffer
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t *row = pixelBuffer[i];
    animateShiftRow(row);
  }
}

// loads a CustomChar into the pixelBuffer
void incrementDrawLength() {
  /* Because each character loaded into the pixel buffer is actually 9 bits
   * wide (becuase of the 1-pixel gap between each character), we add an
   * additional 1 to the draw length for every 8 characters loaded, rounding
   * up.  This addition is not done here, but is instead stored in a global
   * variable and added to drawlength outside this function.*/
  gapDrawLength =
      ((drawLength % 8) == 0) ? (gapDrawLength + 1) : (gapDrawLength);
  /* drawLength is incremented by one each call to this function.  Because
   * drawLength is used during loading of the characters as an index into the
   * pixelBuffer array, we do not add gapDrawLength to drawLength within this
   * function.  No data is lost when indexing like this because the data is
   * shifted to the end of the array, and is not overwritten.*/
  drawLength++;
}

// loads a ascii coded char into the pixelBuffer, from the font loaded in font.h
void loadCharInit(uint8_t curChar) {
  // increment the drawLength each time we load a character
  // this also tracks the additional space needed by the gaps between chars
  incrementDrawLength();
  // load the character from the font
  const CustomChar customChar = getFontChar(curChar);
  // push the character from the font to the pixelBuffer
  for (uint8_t i = 0; i < 8; i++) {
    pixelBuffer[i][drawLength] = (customChar.lines[i]);
  }
  // add a 1 pixel gap after each char in the pixelBuffer
  animateShift();
}

/* charIndex: the starting position of the character to be loaded from
 * pixelBuffer (starting position of character is the same for each row)
 * return: a CustomChar object made by reading from each row at charIndex in
 * pixelBuffer*/
CustomChar generateCustomChar(uint8_t charIndex) {
  CustomChar ret;
  for (uint8_t i = 0; i < 8; i++) {
    ret.lines[i] = pixelBuffer[i][charIndex];
  }
  return ret;
}

// send the first 8 chars in pixelbuffer to LCD, and draw them on the LCD
void drawFrame() {
  // render the pixelBuffer into custom chararcters, and send the custom
  // characters to the LCD to be drawn later
  for (uint8_t i = 0; i < 8; i++) {
    CustomChar curChar = generateCustomChar(i);
    LCDWriteCustomChar(&curChar, i);
  }
  // clear the LCD and set the cursor home
  // clearing and sending the characters to the LCD each
  // time is actually not necessary, as the LCD will update as
  // the CGRAM is written.  It is kept here for reliability
  LCDClear();
  // push the cursor to the center of LCD by drawing spaces
  for (uint8_t i = 0; i < 3; i++) {
    LCDWriteData(' ');  // center the 8 chars that make up our frame
  }
  // tell the LCD to draw each custom character we just loaded on the screen
  // custom characters are stored at addresses 0-7 on the LCD
  for (uint8_t i = 0; i < 8; i++) {
    LCDWriteData(i);
  }
  isRendered = true;
}

// loads the given string into the pixelBuffer
void animateInit(const char *str) {
  // load spaces into the buffer so there is a gap of whitespace before the
  // user string wraps back around in the animation
  for (uint8_t i = 0; i < ANIMATE_SPACE_COUNT; i++) {
    loadCharInit(' ');
  }
  // calculate the length of the string
  const char *tmp = str;
  uint16_t i = 0;
  while (*str != 0) {
    i++;
    str++;
  }
  // check if the given string will fit in the statically allocated
  // pixelBuffer, including the ANIMATE_SPACE_COUNT spaces, and send an error
  // string to the LCD if it will not fit
  if (((i + ANIMATE_SPACE_COUNT) * 9) > (ANIMATE_BUFFER_SIZE * 8)) {
    str = errString;
  } else {
    str = tmp;
  }
  // load each char of the string into the pixelbuffer
  while (*str != 0) {
    loadCharInit(*str);
    str++;
  }
  // becuase we generate a gap between each character by shifting the data in
  // the buffer, each chararcter is 9 bytes wide when loaded in the buffer.
  // This means the length of data in the buffer is wider than incrementing
  // the bufferLength by 1 (an increment by 1 meaning the buffer is 8 bits
  // longer) for each character loaded. To compensate, we add a 1 to the
  // length of the buffer for every 8 characters loaded, rounding up.
  drawLength += gapDrawLength;
}

bool animateDelay() {
  // store the animateDelay() counter statically
  static uint8_t i = ANIMATE_DELAY;
  // create a flag to be flipped by delayFlag
  volatile static bool frameDelayFlag = false;
  delayFlag(&frameDelayFlag, 4000);
  if (frameDelayFlag) {      // true if 4000us have passed
    i++;                     // increment delay counter
    frameDelayFlag = false;  // reset the flag
  }
  // when counter reaches the selected value, return true
  // otherwise, return false
  if (i == ANIMATE_DELAY) {
    i = 0;
    return true;
  }
  return false;
}
// call takes ~4.25ms when drawing a frame, takes only 4.3us when only servicing
// the timer

void animate(const char *str) {
  static bool isInitialized = false;
  if (!isInitialized) {
    LCDInit();             // initialize the LCD
    animateInit(str);      // load the given string into the animation buffer
    isInitialized = true;  // do not initialize animate again
  }
  // draw the frame and shift the bits in the buffer only if ANIMATE_DELAY
  // amount of time has passed
  if (animateDelay()) {
    drawFrame();     // takes 5.57ms, draws the frame on LCD
    animateShift();  // takes 0.5ms when string length is 26
  } else {
    // do nothing if ANIMATE_DELAY has not passed
  }
}
