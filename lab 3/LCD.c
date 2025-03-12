/*
 * LCD.c
 *
 * Created: 3/7/2025 11:16:43 PM
 *  Author: Joe Maloney
 */
#include "LCD.h"

#include "delay.h"
#include "portAPI.h"

// configuration settings used by LCDInit()
struct LCDConfig LCDConfig = {
    .isCursorIncrement = 1,
    .isDisplayShift = 0,
    .isDisplayOn = 1,
    .isCursorOn = 0,
    .isCursorBlinkOn = 0,
    .is8BitData = 1,
    .is2LineMode = 0,
    .is5x11Font = 0,
};

// sends pulse on E to latch the data sent to LCD
inline void LCDLatchData() {
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 1);
  delayMicroseconds(1);  // pulse needs to be at least 150ns long, and at
                         // least 1.2us between pulses
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 0);
}

void LCDCursorHome() {
  LCDWriteCommand(0x2);               // from datasheet
  delayMicroseconds(LCD_LONG_DELAY);  // wait for LCD to process
}

// Clears all LCD communication pins
inline void LCDZeroOutputs() {
  portWritePin(&LCD_RW_PORT, LCD_RW_PIN, 0);
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 0);
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  portWrite(&LCD_DATA_PORT, 0x0);
}

inline void LCDWriteData(uint8_t data) {
  // select data mode on the LCD
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 1);
  portWrite(&LCD_DATA_PORT, data);     // put the data on the data lines
  LCDLatchData();                      // send a pulse on E to latch the data
  delayMicroseconds(LCD_SHORT_DELAY);  // wait for LCD to process
  LCDZeroOutputs();                    // return all LCD pins to zero
}

inline void LCDWriteCommand(uint8_t data) {
  // select command mode on the LCD
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  portWrite(&LCD_DATA_PORT, data);     // put the data on the data lines
  LCDLatchData();                      // send a pulse on E to latch the data
  delayMicroseconds(LCD_SHORT_DELAY);  // wait for LCD to process
  LCDZeroOutputs();                    // return all LCD pins to zero
}

// clears display, and sets cursor at 0,0
inline void LCDClear() {
  LCDWriteCommand(0x1);               // from datasheet
  delayMicroseconds(LCD_LONG_DELAY);  // wait for LCD to process
}

void LCDInit() {
  // configurePins
  for (int i = 0; i <= 7; i++) {
    portConfigOutput(&LCD_DATA_PORT, i);  // set the LCD data port as output
  }
  // set the other LCD pins as outputs
  portConfigOutput(&LCD_LATCH_PORT, LCD_LATCH_PIN);
  portConfigOutput(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN);
  // RW needs to be low for duration of LCD usage
  portConfigOutput(&LCD_RW_PORT, LCD_RW_PIN);
  LCDZeroOutputs();  // clear outputs on LCD pins
  // "function set" from datasheet
  uint8_t tmp = ((1 << 5) | (LCDConfig.is8BitData << 4) |
                 (LCDConfig.is2LineMode << 3) | (LCDConfig.is5x11Font << 2));
  LCDWriteCommand(tmp);
  tmp = (1 << 3);  // "Display ON/OFF" from datasheet
  LCDWriteCommand(tmp);
  // "Entry Mode set" from datasheet
  tmp = ((1 << 2) | (LCDConfig.isCursorIncrement << 1) |
         LCDConfig.isDisplayShift);
  LCDWriteCommand(tmp);
  // "Display ON/OFF" from datasheet
  tmp = ((1 << 3) | (LCDConfig.isDisplayOn << 2) | (LCDConfig.isCursorOn << 1) |
         LCDConfig.isCursorBlinkOn);
  LCDWriteCommand(tmp);
  LCDClear();  // clear the display, and sets the cursor to 0,0
}

void LCDWriteCustomChar(CustomChar *customChar, uint8_t addr) {
  //"Set CGRAM address" from datasheet
  addr = (1 << 6) | (addr << 3);  // this line from arduino library,
                                  // the CGRAM addressing scheme (for writing)
                                  // is not explained in the LCD datasheet
  // set the address of the incoming custom character on the LCD
  LCDWriteCommand(addr);
  // without this additional delay, sometimes not all bytes of the customChar
  // are read by LCD
  delayMicroseconds(LCD_SHORT_DELAY);
  for (uint8_t i = 0; i < 8; i++) {  // send each of the 8 bytes that make up a
                                     // custom character to the LCD
    // only bits 0:5 of each byte are used by the LCD
    LCDWriteData(customChar->lines[i]);
  }
}
