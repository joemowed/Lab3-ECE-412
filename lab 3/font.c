/*
 * font.c
 *
 * Created: 3/8/2025 7:19:05 AM
 *  Author: Joe Maloney
 */
#include "font.h"
#include "LCD.h"
#include "regAPI.h"
#include <stdint-gcc.h>

	// WARNING: This Font is usable only with MikroE GLCD Lib.
	//          X-GLCD Lib does not handle this font.

	// Font Generated by MikroElektronika GLCD Font Creator 1.2.0.0
	// MikroeElektronika 2011
	// http://www.mikroe.com

	// GLCD FontName : Terminal5x8
	// GLCD FontSize : 5 x 8
	const uint8_t Terminal5x8[] = {

		// 0x00, 0x00, 0x00, 0x00, 0x00,            // Code for char
		// 0x00, 0x06, 0x5F, 0x06, 0x00,            // Code for char !
		// 0x03, 0x07, 0x00, 0x03, 0x07,            // Code for char "
		// 0x24, 0x7E, 0x24, 0x7E, 0x24,            // Code for char #
		// 0x00, 0x12, 0x6A, 0x2B, 0x24,            // Code for char $
		// 0x63, 0x64, 0x08, 0x13, 0x63,            // Code for char %
		// 0x50, 0x20, 0x56, 0x49, 0x36,            // Code for char &
		// 0x00, 0x00, 0x03, 0x07, 0x00,            // Code for char '
		// 0x00, 0x00, 0x41, 0x3E, 0x00,            // Code for char (
		// 0x00, 0x00, 0x3E, 0x41, 0x00,            // Code for char )
		// 0x08, 0x3E, 0x1C, 0x3E, 0x08,            // Code for char *
		// 0x08, 0x08, 0x3E, 0x08, 0x08,            // Code for char +
		// 0x00, 0x00, 0x60, 0xE0, 0x00,            // Code for char ,
		// 0x08, 0x08, 0x08, 0x08, 0x08,            // Code for char -
		// 0x00, 0x00, 0x60, 0x60, 0x00,            // Code for char .
		// 0x02, 0x04, 0x08, 0x10, 0x20,            // Code for char /
		// 0x3E, 0x45, 0x49, 0x51, 0x3E,            // Code for char 0
		// 0x00, 0x40, 0x7F, 0x42, 0x00,            // Code for char 1
		// 0x46, 0x49, 0x49, 0x51, 0x62,            // Code for char 2
		// 0x36, 0x49, 0x49, 0x49, 0x22,            // Code for char 3
		// 0x10, 0x7F, 0x12, 0x14, 0x18,            // Code for char 4
		// 0x31, 0x49, 0x49, 0x49, 0x2F,            // Code for char 5
		// 0x30, 0x49, 0x49, 0x4A, 0x3C,            // Code for char 6
		// 0x03, 0x05, 0x09, 0x71, 0x01,            // Code for char 7
		// 0x36, 0x49, 0x49, 0x49, 0x36,            // Code for char 8
		// 0x1E, 0x29, 0x49, 0x49, 0x06,            // Code for char 9
		// 0x00, 0x00, 0x6C, 0x6C, 0x00,            // Code for char :
		// 0x00, 0x00, 0x6C, 0xEC, 0x00,            // Code for char ;
		// 0x00, 0x41, 0x22, 0x14, 0x08,            // Code for char <
		// 0x24, 0x24, 0x24, 0x24, 0x24,            // Code for char =
		// 0x08, 0x14, 0x22, 0x41, 0x00,            // Code for char >
		// 0x06, 0x09, 0x59, 0x01, 0x02,            // Code for char ?
		// 0x1E, 0x55, 0x5D, 0x41, 0x3E,            // Code for char @
		0x7E, 0x11, 0x11, 0x11, 0x7E, // Code for char A
		0x36, 0x49, 0x49, 0x49, 0x7F, // Code for char B
		0x22, 0x41, 0x41, 0x41, 0x3E, // Code for char C
		0x3E, 0x41, 0x41, 0x41, 0x7F, // Code for char D
		0x41, 0x49, 0x49, 0x49, 0x7F, // Code for char E
		0x01, 0x09, 0x09, 0x09, 0x7F, // Code for char F
		0x7A, 0x49, 0x49, 0x41, 0x3E, // Code for char G
		0x7F, 0x08, 0x08, 0x08, 0x7F, // Code for char H
		0x00, 0x41, 0x7F, 0x41, 0x00, // Code for char I
		0x3F, 0x40, 0x40, 0x40, 0x30, // Code for char J
		0x41, 0x22, 0x14, 0x08, 0x7F, // Code for char K
		0x40, 0x40, 0x40, 0x40, 0x7F, // Code for char L
		0x7F, 0x02, 0x04, 0x02, 0x7F, // Code for char M
		0x7F, 0x08, 0x04, 0x02, 0x7F, // Code for char N
		0x3E, 0x41, 0x41, 0x41, 0x3E, // Code for char O
		0x06, 0x09, 0x09, 0x09, 0x7F, // Code for char P
		0x5E, 0x21, 0x51, 0x41, 0x3E, // Code for char Q
		0x66, 0x19, 0x09, 0x09, 0x7F, // Code for char R
		0x32, 0x49, 0x49, 0x49, 0x26, // Code for char S
		0x01, 0x01, 0x7F, 0x01, 0x01, // Code for char T
		0x3F, 0x40, 0x40, 0x40, 0x3F, // Code for char U
		0x1F, 0x20, 0x40, 0x20, 0x1F, // Code for char V
		0x3F, 0x40, 0x3C, 0x40, 0x3F, // Code for char W
		0x63, 0x14, 0x08, 0x14, 0x63, // Code for char X
		0x07, 0x08, 0x70, 0x08, 0x07, // Code for char Y
		0x00, 0x43, 0x45, 0x49, 0x71, // Code for char Z
		0x00, 0x41, 0x41, 0x7F, 0x00, // Code for char [
		0x20, 0x10, 0x08, 0x04, 0x02, // Code for char BackSlash
		0x00, 0x7F, 0x41, 0x41, 0x00, // Code for char ]
		0x04, 0x02, 0x01, 0x02, 0x04, // Code for char ^
		0x80, 0x80, 0x80, 0x80, 0x80, // Code for char _
		0x00, 0x00, 0x07, 0x03, 0x00, // Code for char `
		0x78, 0x54, 0x54, 0x54, 0x20, // Code for char a
		0x38, 0x44, 0x44, 0x44, 0x7F, // Code for char b
		0x28, 0x44, 0x44, 0x44, 0x38, // Code for char c
		0x7F, 0x44, 0x44, 0x44, 0x38, // Code for char d
		0x08, 0x54, 0x54, 0x54, 0x38, // Code for char e
		0x00, 0x09, 0x09, 0x7E, 0x08, // Code for char f
		0x7C, 0xA4, 0xA4, 0xA4, 0x18, // Code for char g
		0x00, 0x78, 0x04, 0x04, 0x7F, // Code for char h
		0x00, 0x40, 0x7D, 0x00, 0x00, // Code for char i
		0x00, 0x7D, 0x84, 0x80, 0x40, // Code for char j
		0x00, 0x44, 0x28, 0x10, 0x7F, // Code for char k
		0x00, 0x40, 0x7F, 0x00, 0x00, // Code for char l
		0x78, 0x04, 0x18, 0x04, 0x7C, // Code for char m
		0x00, 0x78, 0x04, 0x04, 0x7C, // Code for char n
		0x38, 0x44, 0x44, 0x44, 0x38, // Code for char o
		0x38, 0x44, 0x44, 0x44, 0xFC, // Code for char p
		0xFC, 0x44, 0x44, 0x44, 0x38, // Code for char q
		0x08, 0x04, 0x44, 0x78, 0x44, // Code for char r
		0x20, 0x54, 0x54, 0x54, 0x08, // Code for char s
		0x00, 0x24, 0x44, 0x3E, 0x04, // Code for char t
		0x00, 0x7C, 0x20, 0x40, 0x3C, // Code for char u
		0x1C, 0x20, 0x40, 0x20, 0x1C, // Code for char v
		0x3C, 0x60, 0x30, 0x60, 0x3C, // Code for char w
		0x00, 0x6C, 0x10, 0x10, 0x6C, // Code for char x
		0x00, 0x3C, 0x60, 0xA0, 0x9C, // Code for char y
		0x00, 0x4C, 0x54, 0x54, 0x64, // Code for char z
		0x00, 0x41, 0x41, 0x3E, 0x08, // Code for char {
		0x00, 0x00, 0x77, 0x00, 0x00, // Code for char |
		0x08, 0x3E, 0x41, 0x41, 0x00, // Code for char }
		0x00, 0x01, 0x02, 0x01, 0x02, // Code for char ~

	};
CustomChar decompressChar(const uint8_t *compressedStart)
{
	CustomChar ret = {{0, 0, 0, 0, 0, 0, 0, 0}};
	for (uint8_t currentCharacter = 0; currentCharacter < 5; currentCharacter++)
	{
		for (uint8_t curLine = 0; curLine < 8; curLine++)
		{
			volatile uint8_t msk = (READ_BIT(compressedStart[currentCharacter], curLine));
			msk = (msk > 0);
			msk <<= currentCharacter;
			ret.lines[curLine] |= msk;
		}
	}
	return ret;
}
CustomChar getFontChar(char curChar){
	uint16_t index = curChar -65;
	index*=5;
	return decompressChar(&Terminal5x8[index]);
}

