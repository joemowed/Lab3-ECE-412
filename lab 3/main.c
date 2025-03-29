
// Lab3P1.c
//
//
// Author : Eugene Rockey
//

//

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
	const char uart_config_msg[] = "UART settings will be changed.  Change terra-term settings to 9600 baud, 2 stop bits, and 8 data bits.\n Press any key when terra-term settings are established to continue.";
	UART_Puts(uart_config_msg);
	UART_Init(103U);// set new uart settings
	UART_Get();// wait for user input
	while(1)
	{
		if(RXFlag){
			break;
		}
		animate("Arvato",&animateStacking,&LCDStacking);
	}
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
			UART_Puts("This str");
			break;
			case 'E' | 'e':
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

	int main(void) {
		UCSR0B = (0<<RXEN0)|(0<<TXEN0);
		UART_Init(103U);
		Banner();
		while(1){
			Command();
		}
		}
