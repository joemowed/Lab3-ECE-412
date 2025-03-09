/*
* font.h
*
* Created: 3/8/2025 7:15:25 AM
*  Author: Joe Maloney
*/


#ifndef FONT_H_
#define FONT_H_


#include "LCD.h"
#define FONT_CHAR_COUNT 62
CustomChar fontArray[FONT_CHAR_COUNT];
void decompressFont();
inline CustomChar* getFontChar(uint8_t character){
	return &(fontArray[(character-65)]);
}



#endif /* FONT_H_ */