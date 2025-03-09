/*
* animate.c
*
* Created: 3/8/2025 8:57:12 AM
*  Author: Joe Maloney
*/
#include "animate.h"
#include "LCD.h"
#include "delay.h"
#include "font.h"
#include "regAPI.h"
#include <stdbool.h>
#include <stdint-gcc.h>
static bool isInitialized = false;
static uint8_t pixelBuffer[8][ANIMATE_BUFFER_SIZE]; //2d array, pixelBuffer[0:7] holds the rows of pixels to be sent to drawFrame
static uint8_t drawLength = 0;
//carryIn: value to be shifted into LSB of value
//value: shifts this value 1 pos to the left and writes carryIn to LSB
//return: The MSB of the initial value
bool LSLCarry(uint8_t *value,bool carryIn) {
	bool ret = READ_BIT((*value),4);//Font is only 5 pixels (bits) wide, so we take the carry as bit #4 (the 5th bit of the byte).
	*value<<=1;
	*value |= carryIn;
	return ret;
}
//loads a CustomChar into the pixelBuffer
void loadCharInit(uint8_t curChar){
	const CustomChar *customChar = getFontChar(curChar);
	for(uint8_t i = 0;i<8;i++){
		pixelBuffer[i][drawLength] = (customChar->lines[i]);
	}
	drawLength++;
	animateShift();//add a 1 pixel gap after each char in the pixelBuffer
}
//helper function for animateShift
void animateShiftRow(uint8_t*rowStart){
	bool MSBCarry = READ_BIT(rowStart[0],7);//MSB of the start of row
	int8_t i = drawLength-1;
	bool carry = LSLCarry(&rowStart[i],MSBCarry);
	i--;
	for(;i>=0;i--){
		carry = LSLCarry(&rowStart[i],carry);
	}
}
//shifts all bits in the buffer 1 place to the left, rotating the MSB to the LSB
void animateShift(){
	for(uint8_t i = 0; i<8;i++){
		uint8_t * row = pixelBuffer[i];
		animateShiftRow(row);
	}
}
//charStart: pointer to first byte of character in pixelBuffer
//return: a CustomChar object made by reading from charStart to charStart+7 in pixelBuffer
CustomChar generateCustomChar(uint8_t charIndex){
	CustomChar ret;
	for(uint8_t i =0;i<8;i++){
		ret.lines[i] = pixelBuffer[i][charIndex];
	}
	return ret;
}
//send the first 8 chars in pixelbuffer to LCD, and draw them on the LCD
void drawFrame(){
	for(uint8_t i =0;i<8;i++){
		CustomChar curChar = generateCustomChar(i);
		LCDWriteCustomChar(&curChar,i);
	}
	//	LCDCursorHome();
	LCDClear();
	for(uint8_t i = 0;i<8;i++){
		LCDWriteData(i);
	}
	
}
void animateInit(const uint8_t*str){
	isInitialized = true;
	for(uint8_t i =0;i<ANIMATE_SPACE_COUNT;i++){
		loadCharInit(' ');
	}
	while(*str!=0){
		loadCharInit(*str);
		str++;
	}


}
void animateDelay(){
	for(uint8_t i = 0;i<ANIMATE_DELAY;i++){
		delayMicroseconds(4000);
	}
}
void animate(const uint8_t*str){
	if(!isInitialized){
	decompressFont();
	LCDInit();
		animateInit(str);
	}
	drawFrame();
	animateShift();
	animateDelay();
}
