/*
 * LCD.h
 *
 * Created: 3/7/2025 10:54:31 PM
 *  Author: Joe Maloney
 */

#ifndef LCD_H_
#define LCD_H_

#include "pinstacking.h"
#include <avr/io.h>     //used in this file to define the ports below
#include <stdbool.h>    //
#include <stdint-gcc.h> //import the nessasary types for declarations

#define LCD_DATA_PORT PORTD       // the port used for LCD data pins 0:7
#define LCD_REG_SELECT_PIN 1      // the pin used for RS on the LCD
#define LCD_REG_SELECT_PORT PORTB // the port associated with the RS pin
#define LCD_RW_PIN 0              // the pin used for read/write on the LCD
#define LCD_RW_PORT PORTB         // the port associated with the read/write pin
#define LCD_LATCH_PIN 2           // the pin used for the LCD pin E
#define LCD_LATCH_PORT PORTB      // the port associated with the E pin

// microseconds, used for return home and clear display
#define LCD_LONG_DELAY 1600

// microseconds, used for all other LCD instructions
#define LCD_SHORT_DELAY 40

// writes the given uint8_t to the LCD as data
void LCDWriteData(uint8_t data);

// writes the given uint8_t to the LCD as a command
void LCDWriteCommand(uint8_t data);

// sends the "Clear" command to the LCD
void LCDClear();

// enables and configures LCD as defined in the LCDConfigStruct
void LCDInit(PSCallbacks *LCDStacking);

// typedef for a custom character.
// on the 1602 LCD, custom characters are 5x8 pixels
// we use bits 0:5 to define each row in a custom character
typedef struct CustomCharStruct {
    uint8_t lines[8];
} CustomChar;

// writes a custom character to the LCD at the address specified, using
// addresses 0-7, inclusive
void LCDWriteCustomChar(CustomChar *custChar, uint8_t addr);

// sends the cursor to 0,0 on the LCD
void LCDCursorHome();

// sets the appropriate pins to outputs for driving the LCD
void LCDConfigPins();

// configuration for the LCD
// This is used by the LCDInit() function to
// configure the display
// the actual configuration is in LCD.c
struct LCDConfig {
    bool isCursorIncrement;

    bool isDisplayShift;
    bool isDisplayOn;
    bool isCursorOn;
    bool isCursorBlinkOn;
    bool is8BitData;
    bool is2LineMode;
    bool is5x11Font;
};

#endif /* LCD_H_ */
