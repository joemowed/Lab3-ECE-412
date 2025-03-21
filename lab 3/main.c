
// Lab3P1.c
//
//
// Author : Eugene Rockey
//

//

const char MS1[] = "\r\nECE-412 ATMega328PB Tiny OS";
const char MS2[] = "\r\nby Eugene Rockey Copyright 2022, All Rights Reserved";
const char MS3[] = "\r\nMenu: (L)CD, (A)DC, (E)EPROM\r\n";
const char MS4[] = "\r\nReady: ";
const char MS5[] = "\r\nInvalid Command Try Again...";
const char MS6[] = "Volts\r";

void LCD_Init(void);  // external Assembly functions
//void UART_Init(void);
void UART_Clear(void);
void UART_Get(void);
void UART_Put(void);
void LCD_Write_Data(void);
void LCD_Write_Command(void);
void LCD_Read_Data(void);
void Mega328P_Init(void);
void ADC_Get(void);
void EEPROM_Read(void);
void EEPROM_Write(void);

unsigned char ASCII;  // shared I/O variable with Assembly
unsigned char DATA;   // shared internal variable with Assembly
char HADC;            // shared ADC variable with Assembly
char LADC;            // shared ADC variable with Assembly

char volts[5];  // string buffer for ADC output
int Acc;        // Accumulator for ADC use
#include "regAPI.h"
void UART_Puts(const char *str)  // Display a string in the PC Terminal Program
{
	while (*str) {
		ASCII = *str++;
		saveRegs();
		UART_Put();
		restoreRegs();
	}
}

void LCD_Puts(const char *str)  // Display a string on the LCD Module
{
	while (*str) {
		DATA = *str++;
		LCD_Write_Data();
	}
}

void Banner(void)  // Display Tiny OS Banner on Terminal
{
	UART_Puts(MS1);
	UART_Puts(MS2);
	UART_Puts(MS4);
}

void HELP(void)  // Display available Tiny OS Commands on Terminal
{
	UART_Puts(MS3);
}

void LCD(void)  // Lite LCD demo
{
	DATA = 0x34;  // Student Comment Here
	LCD_Write_Command();
	DATA = 0x08;  // Student Comment Here
	LCD_Write_Command();
	DATA = 0x02;  // Student Comment Here
	LCD_Write_Command();
	DATA = 0x06;  // Student Comment Here
	LCD_Write_Command();
	DATA = 0x0f;  // Student Comment Here
	LCD_Write_Command();
	LCD_Puts("Hello ECE412!");
	/*
	Re-engineer this subroutine to have the LCD endlessly scroll a marquee sign of
	your Team's name either vertically or horizontally. Any key press should stop
	the scrolling and return execution to the command line in Terminal. User must
	always be able to return to command line.
	*/
}

void ADC(void)  // Lite Demo of the Analog to Digital Converter
{
	volts[0x1] = '.';
	volts[0x3] = ' ';
	volts[0x4] = 0;
	ADC_Get();
	Acc = (((int)HADC) * 0x100 + (int)(LADC)) * 0xA;
	volts[0x0] = 48 + (Acc / 0x7FE);
	Acc = Acc % 0x7FE;
	volts[0x2] = ((Acc * 0xA) / 0x7FE) + 48;
	Acc = (Acc * 0xA) % 0x7FE;
	if (Acc >= 0x3FF) volts[0x2]++;
	if (volts[0x2] == 58) {
		volts[0x2] = 48;
		volts[0x0]++;
	}
	UART_Puts(volts);
	UART_Puts(MS6);
	/*
	Re-engineer this subroutine to display temperature in degrees Fahrenheit on
	the Terminal. The potentiometer simulates a thermistor, its varying resistance
	simulates the varying resistance of a thermistor as it is heated and cooled.
	See the thermistor equations in the lab 3 folder. User must always be able to
	return to command line.
	*/
}

void EEPROM(void) {
	UART_Puts("\r\nEEPROM Write and Read.");
	/*
	Re-engineer this subroutine so that a byte of data can be written to any
	address in EEPROM during run-time via the command line and the same byte of
	data can be read back and verified after the power to the Xplained Mini board
	has been cycled. Ask the user to enter a valid EEPROM address and an 8-bit
	data value. Utilize the following two given Assembly based drivers to
	communicate with the EEPROM. You may modify the EEPROM drivers as needed. User
	must be able to always return to command line.
	*/
	UART_Puts("\r\n");
	EEPROM_Write();
	UART_Puts("\r\n");
	EEPROM_Read();
	UART_Put();
	UART_Puts("\r\n");
}

void Command(void)  // command interpreter
{
	UART_Puts(MS3);
	ASCII = '\0';
	while (ASCII == '\0') {
		UART_Get();
	}
	switch (ASCII) {
		case 'L' | 'l':
		LCD();
		break;
		case 'A' | 'a':
		ADC();
		break;
		case 'E' | 'e':
		EEPROM();
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;
		// Add a 'USART' command and subroutine to allow the user to reconfigure
		// the serial port parameters during runtime. Modify baud rate, # of data
		// bits, parity, # of stop bits.
	}
}

#include "animate.h"
#include "delay.h"
#include "portAPI.h"
#include "profiling.h"
#include "pinstacking.h"
#include "termcontrol.h"
#include <avr/io.h>
void UART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*	Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void delayLoop(uint16_t cnt){
//cnt = 6000 is ~2.6ms with -O0
//cnt = 3000 is ~1.3ms with -O0
	for(uint16_t i =0;i<cnt;i++){
		asm("nop");
	}
}
void enableUARTCallback(){
	UART_Init(103U);
	delayLoop(20000);
	UART_Puts(TTCSI"u");
	UART_Puts(TTCSI"0J");
	delayLoop(10000);
}
void disableUARTCallback(){
	delayLoop(20000);
	UART_Puts(TTCSI"s");
	UCSR0B = (0<<RXEN0)|(0<<TXEN0);
	delayLoop(10000);
}

const PSCallbacks animateStacking= {
	enableUARTCallback,
disableUARTCallback};
//pcint16
void enableReceiveFlag(){
PCMSK2 = (1<<PCINT16);
PCICR = (1<<PCIE2);
}
void disableReceiveFlag(){
PCICR = 0x0;
}
void nullfunc(){
	return;
}
//static PSCallbacks nullPS = {nullfunc,nullfunc};
PSCallbacks LCDStacking = {enableReceiveFlag,disableReceiveFlag};
int main(void) {
	UCSR0B = (0<<RXEN0)|(0<<TXEN0);
	UART_Init(103U);
	Banner();
	while (1) {
		if(animate("Arvato",&animateStacking,&LCDStacking)){
			asm("nop");
		}
	}
}
