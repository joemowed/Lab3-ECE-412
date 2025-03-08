/*
* LCD.h
*
* Created: 3/7/2025 10:54:31 PM
*  Author: Joe Maloney
*/


#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#define LCD_DATA_PORT PORTD
#define LCD_REG_SELECT_PIN 1
#define LCD_REG_SELECT_PORT PORTB
#define LCD_RW_PIN 0
#define LCD_RW_PORT PORTB
#define LCD_LATCH_PIN 2
#define LCD_LATCH_PORT PORTB
//microseconds, used for return home and clear display
#define LCD_LONG_DELAY 1600
//microseconds, used for all other LCD instructions
#define LCD_SHORT_DELAY 40
void LCDWriteData(uint8_t data);
void LCDWriteCommand(uint8_t data);
void LCDClear();
//turns display on, cursor off
void LCDInit();
typedef struct CustomCharStruct{uint8_t lines[8];} CustomChar;
//use numbers 0-7 as addresses
void LCDWriteCustomChar(CustomChar*custChar,uint8_t addr);
void LCDCursorHome();
struct LCDConfig{
	bool isCursorIncrement;
	bool isDisplayShift ;
	bool isDisplayOn ;
	bool isCursorOn ;
	bool isCursorBlinkOn ;
	bool is8BitData ;
	bool is2LineMode ;
	bool is5x11Font ;
};



#endif /* LCD_H_ */