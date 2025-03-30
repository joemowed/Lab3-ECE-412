
// Lab3P1.c
//
//
// Author : Eugene Rockey
//

//

#include "animate.h"
#include "pinstacking.h"
#include "regAPI.h"

 const char MS1[] = "\r\nECE-412 ATMega328PB Tiny OS";
 const char MS2[] = "\r\nby Eugene Rockey Copyright 2022, All Rights Reserved";
 const char MS3[] = "\r\nMenu: (L)CD, (A)DC, (E)EPROM, (U)SART\r\n";
 const char MS4[] = "\r\nReady: ";
 const char MS5[] = "\r\nInvalid Command Try Again...";
 const char MS6[] = "Volts\r";
 const char MS7[] = "\r\n change: (1)baud rate, (2)# of bits, (3) parity, (4) # stop bits\r\n";
 const char MS8[] = "\r\n baud rate: (1) 4800 or (2) 9600\r\n";
 const char MS9[] = "\r\n # of bits: (5), (6), (7), (8), (9)\r\n";
 const char MS10[] = "\r\n parity: (E)nabled,even or (d)isabled\r\n";
 const char MS11[] = "\r\n # stop bits: (1)  or (2) \r\n";
 
 
 

void LCD_Init(void);			//external Assembly functions
void UART_Init(void);
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
void UART_Stop(void);
void UART_Parity(void);
void UART_Bits(void);
void UART_Baud(void);



unsigned char ASCII;			//shared I/O variable with Assembly
unsigned char DATA;				//shared internal variable with Assembly
char HADC;						//shared ADC variable with Assembly
char LADC;						//shared ADC variable with Assembly

char volts[5];					//string buffer for ADC output
int Acc;						//Accumulator for ADC use

void UART_Puts(const char *str)	//Display a string in the PC Terminal Program
{
	while (*str)
	{
		ASCII = *str++;
		wrapASM(UART_Put);
	}
}




void Banner(void)				//Display Tiny OS Banner on Terminal
{
	UART_Puts(MS1);
	UART_Puts(MS2);
	UART_Puts(MS4);
}

void HELP(void)						//Display available Tiny OS Commands on Terminal
{
	UART_Puts(MS3);
}


void LCD(void)  // Lite LCD demo
{
	const char uart_config_msg[] = "UART settings will be changed."
	"Change terra-term settings to 9600 baud, 2 stop bits, and 8 data bits.\r\n"
	 "Press any key when terra-term settings are established to continue.\r\n"
	 "Press any key to exit the animation and return to command line.\r\n";
	UART_Puts(uart_config_msg);
	UARTInitPS(103U);// set new uart settings
	wrapASM(UART_Get);// wait for user input
	while(1)
	{
		if(RXFlag){
			break;
		}
		animate("Arvato",&animateStacking,&LCDStacking);
	}
	animateStop();
	return;
}

void ADCCommand(void)						//Lite Demo of the Analog to Digital Converter
{
	volts[0x1]='.';
	volts[0x3]=' ';
	volts[0x4]= 0;
	ADC_Get();
	Acc = (((int)HADC) * 0x100 + (int)(LADC))*0xA;
	volts[0x0] = 48 + (Acc / 0x7FE);
	Acc = Acc % 0x7FE;
	volts[0x2] = ((Acc *0xA) / 0x7FE) + 48;
	Acc = (Acc * 0xA) % 0x7FE;
	if (Acc >= 0x3FF) volts[0x2]++;
	if (volts[0x2] == 58)
	{
		volts[0x2] = 48;
		volts[0x0]++;
	}
	UART_Puts(volts);
	UART_Puts(MS6);
	/*
		Re-engineer this subroutine to display temperature in degrees Fahrenheit on the Terminal.
		The potentiometer simulates a thermistor, its varying resistance simulates the
		varying resistance of a thermistor as it is heated and cooled. See the thermistor
		equations in the lab 3 folder. User must always be able to return to command line.
	*/
	
}

void EEPROM(void)
{
	UART_Puts("\r\nEEPROM Write and Read.");
	/*
	Re-engineer this subroutine so that a byte of data can be written to any address in EEPROM
	during run-time via the command line and the same byte of data can be read back and verified after the power to
	the Xplained Mini board has been cycled. Ask the user to enter a valid EEPROM address and an
	8-bit data value. Utilize the following two given Assembly based drivers to communicate with the EEPROM. You
	may modify the EEPROM drivers as needed. User must be able to always return to command line.
	*/
	UART_Puts("\r\n");
	EEPROM_Write();
	UART_Puts("\r\n");
	EEPROM_Read();
	UART_Put();
	UART_Puts("\r\n");
}
void USART(void)
{
	UART_Puts(MS7);
	ASCII = '\0';
	while (ASCII == '\0')
	{
		UART_Get();
	}
	switch (ASCII)
	{
		case '1':
		UART_Puts(MS8);
		ASCII = '\0';
		while (ASCII == '\0')
		{
			UART_Get();
		}
		switch (ASCII)
		{
			case '1':
			DATA = 0x00;
			UART_Baud();
			break;
			case '2':
			DATA = 0x01;
			UART_Baud();
			break;
			default:
			UART_Puts(MS5);
			break;
		}
		break;
		case '2':
		UART_Puts(MS9);
		ASCII = '\0';
		while (ASCII == '\0')
		{
			UART_Get();
		}
		switch (ASCII)
		{
			case '5':
			DATA = 0x00;
			UART_Bits();
			break;
			case '6':
			DATA = 0x01;
			UART_Bits();
			break;
			case '7':
			DATA = 0x02;
			UART_Bits();
			break;
			case '8':
			DATA = 0x03;
			UART_Bits();
			break;
			case '9':
			DATA = 0x04;
			UART_Bits();
			break;
			default:
			UART_Puts(MS5);
			break;
		}
		break;
		case '3':
		UART_Puts(MS10);
		ASCII = '\0';
		while (ASCII == '\0')
		{
			UART_Get();
		}
		switch (ASCII)
		{
			case 'E' | 'e':
				DATA = 0x00;
				UART_Parity();
			break;
			case 'D' | 'd':
				DATA = 0x01;
				UART_Parity();
			break;
			default:
			UART_Puts(MS5);
			break;
		}
		break;
		case '4':
		UART_Puts(MS11);
		ASCII = '\0';
		while (ASCII == '\0')
		{
			UART_Get();
		}
		switch (ASCII)
		{
			case '1':
				DATA = 0x00;
				UART_Stop();
			break;
			case '2':
				DATA = 0x01;
				UART_Stop();
			break;
			default:
			UART_Puts(MS5);
			break;
		}
		break;
		default:
		UART_Puts(MS5);
		break;
	}
}


void Command(void)					//command interpreter
{
	UART_Puts(MS3);
	ASCII = '\0';						
	while (ASCII == '\0')
	{
		wrapASM(UART_Get);
	}
	switch (ASCII)
	{
		case 'L': 
		case 'l':
		 LCD();
		break;
		case 'A' | 'a': ADCCommand();
		break;
		case 'E' | 'e': EEPROM();
		break;
		case 'U' | 'u'  : USART();
		break;
		default:
		UART_Puts(MS5);
		HELP();
		break;  			
	}
}
int main(void)
{

	wrapASM(Mega328P_Init);
	Banner();
	while (1)
	{
		Command();				//infinite command loop
	}
}




