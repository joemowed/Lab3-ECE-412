/*
* animate.c
*
* Created: 3/8/2025 8:57:12 AM
*  Author: Joe Maloney
*/
#include "LCD.h"
#include "delay.h"
#include "animate.h"
#include "regAPI.h"
#include <stdbool.h>
static CustomChar currentBuffer[ANIMATE_BUFFER_SIZE];
static CustomChar nextBuffer[ANIMATE_BUFFER_SIZE];
static uint8_t strLength =0;
void loadNextBuffer(){
	for(uint8_t i =0;i<strLength;i++){
		currentBuffer[i]=nextBuffer[i];
	}
}
inline bool BSLWithCarry(uint8_t *value){
	bool ret= READ_BIT(value,7);//true if MSB is set
	(*value)<<=1;
	return ret;
}

void animate(uint8_t*str){
uint8_t curChar = str[0];
for(uint8_t i =0;curChar!=0;i++){
	nextBuffer[i] = *(getFontChar(curChar));
	curChar = str[i];
	strLength = i;
}

}
