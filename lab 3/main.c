
// Lab3P1.c
//
//
// Author : Eugene Rockey
//

//

#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>

#include "animate.h"
#include "pinstacking.h"
#include "regAPI.h"

const char MS1[] = "\r\nECE-412 ATMega328PB Tiny OS";
const char MS2[] = "\r\nby Eugene Rockey Copyright 2022, All Rights Reserved";
const char MS3[] =
    "\r\nMenu: (L)CD, (A)DC, (W)Write EPROM, (R)Read EPROM, (U)SART\r\n";

const char MS4[] = "\r\nReady: ";
const char MS5[] = "\r\nInvalid Command Try Again...";
const char MS6[] = "Volts\r";
const char MS7[] =
    "\r\n change: (1)baud rate, (2)# of bits, (3) parity, (4) # stop bits\r\n";
const char MS8[] = "\r\n baud rate: (1) 4800 or (2) 9600\r\n";
const char MS9[] = "\r\n # of bits: (5), (6), (7), (8), (9)\r\n";
const char MS10[] = "\r\n parity: (E)nabled,even or (d)isabled\r\n";
const char MS11[] = "\r\n # stop bits: (1)  or (2) \r\n";

const char MS_enter_addr[] = "enter address:\r\n";
const char MS_enter_byte[] = "enter byte:\r\n";

void LCD_Init(void);  // external Assembly functions
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

unsigned char ASCII;  // shared I/O variable with Assembly
unsigned char DATA;   // shared internal variable with Assembly
char HADC;            // shared ADC variable with Assembly
char LADC;            // shared ADC variable with Assembly
unsigned char byte;

char volts[5];  // string buffer for ADC output
int Acc;        // Accumulator for ADC use

void UART_Puts(const char *str)  // Display a string in the PC Terminal Program
{
  while (*str) {
    ASCII = *str++;
    wrapASM(UART_Put);
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
  const char uart_config_msg[] =
      "UART settings will be changed.  "
      "Change terra-term settings to 9600 baud, 2 stop bits, and 8 data "
      "bits.\r\n"
      "Press any key when terra-term settings are established to continue.\r\n"
      "Press any key to exit the animation and return to command line.\r\n";
  UART_Puts(uart_config_msg);
  UARTInitPS(103U);   // set new uart settings
  wrapASM(UART_Get);  // wait for user input
  RXFlag = false;
  while (1) {
    if (RXFlag) {
      break;
    }
    animate("The Bug Busters", &animateStacking, &LCDStacking);
  }
  animateStop();
  return;
}

void USART(void) {
  UART_Puts(MS7);
  ASCII = '\0';
  while (ASCII == '\0') {
    wrapASM(UART_Get);
  }
  switch (ASCII) {
    case '1':
      UART_Puts(MS8);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      switch (ASCII) {
        case '1':
          DATA = 0x00;
          wrapASM(UART_Baud);
          break;
        case '2':
          DATA = 0x01;
          wrapASM(UART_Baud);
          break;
        default:
          UART_Puts(MS5);
          break;
      }
      break;
    case '2':
      UART_Puts(MS9);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      switch (ASCII) {
        case '7':
          DATA = 0x02;
          wrapASM(UART_Bits);
          break;
        case '8':
          DATA = 0x03;
          wrapASM(UART_Bits);
          break;
      }
      break;
    case '3':
      UART_Puts(MS10);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      switch (ASCII) {
        case 'E':
        case 'e':
          DATA = 0x00;
          wrapASM(UART_Parity);
          break;
        case 'D':
        case 'd':
          DATA = 0x01;
          wrapASM(UART_Parity);
          break;
        default:
          UART_Puts(MS5);
          break;
      }
      break;
    case '4':
      UART_Puts(MS11);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      switch (ASCII) {
        case '1':
          DATA = 0x00;
          wrapASM(UART_Stop);
          break;
        case '2':
          DATA = 0x01;
          wrapASM(UART_Stop);
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

float NTC_Temperature(uint16_t adc_value) {
  // Constants for 10K NTC Thermistor with B3950
  const float BETA = 3950.0;    // Beta value
  const float R_REF = 10000.0;  // Reference resistance
  const float T_REF = 298.15;   // Reference temperature in Kelvin (25 �C)
  const float R_INFINITY = R_REF * exp(-BETA / T_REF);

  // Calculate the resistance of the thermistor
  float resistance = (1023.0 / adc_value - 1.0) * R_REF;

  // Calculate temperature in Kelvin and convert to Celsius
  float temperature_kelvin = BETA / (log(resistance / R_INFINITY));
  float temperature_celsius = temperature_kelvin - 273.15;

  return temperature_celsius;
}

void ADC_Init_Free_Running(void) {
  // Configure ADC for free-running mode
  ADMUX = (1 << REFS0);    // Reference voltage = AVCC, input channel = ADC0
  ADCSRA = (1 << ADEN) |   // Enable ADC
           (1 << ADATE) |  // Enable auto-trigger (free-running mode)
           (1 << ADSC) |   // Start conversion
           (1 << ADPS2) |  // Prescaler = 64 for 125 kHz ADC clock (with 8 MHz
           // system clock)
           (1 << ADPS1);
  ADCSRB = 0x00;  // Free-running mode
}
void ADC_Read(void) {
  UART_Puts("\r\nPress any key to exit.  \r\nTemperature (C): ");
  ADC_Init_Free_Running();  // Initialize ADC in free-running mode
  _delay_ms(1000);
  UART_Puts("\r\n");  // insert newline for the temperature
  while (1) {
    // Proper ADC read for 10-bit value
    uint16_t adc_value = ADCW;  // Use ADCW instead of ADC

    if (adc_value == 0) {
      UART_Puts("\rError: ADC Value = 0");

      continue;  // Prevent division by zero
    }

    volatile float temperature = NTC_Temperature(adc_value);

    char temp_str[10];
    sprintf(temp_str, "%.2f", temperature);

    // Display temperature on terminal
    UART_Puts("\rTemp: ");
    UART_Puts(temp_str);
    UART_Puts(" C ");

    _delay_ms(1000);

    // Check for key press to exit
    if (READ_BIT(UCSR0A,RXC0)) {  // checks if new data has been received in UART
      wrapASM(UART_Get);   // clear the UART data register
      ASCII =
          '\0';  // clear the global flag before returning to the command loop
      return;
    }
	// overwrite the current line with spaces
    UART_Puts("\r                      "); 
  }
}
void WEEPROM(void) {
  UART_Puts("\r\nEEPROM Write.");
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
  wrapASM(EEPROM_Write);
  UART_Puts("\r\n");
}

void REEPROM(void) {
  UART_Puts("\r\nEEPROM Read.");
  UART_Puts("\r\n");
  wrapASM(EEPROM_Read);
  wrapASM(UART_Put);
  UART_Puts("\r\n");
}

void Command(void)  // command interpreter
{
  UART_Puts(MS3);
  ASCII = '\0';
  while (ASCII == '\0') {
    wrapASM(UART_Get);
  }
  switch (ASCII) {
    case 'L':
    case 'l':
      LCD();
      break;

    case 'A':
    case 'a':
      ADC_Read();
      break;

    case 'U':
    case 'u':
      USART();
      break;

    case 'W':
    case 'w':
      UART_Puts(MS_enter_addr);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      DATA = ASCII - 0x30;
      UART_Puts(MS_enter_byte);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      byte = ASCII;
      WEEPROM();
      break;

    case 'R':
    case 'r':
      UART_Puts(MS_enter_addr);
      ASCII = '\0';
      while (ASCII == '\0') {
        wrapASM(UART_Get);
      }
      DATA = ASCII - 0x30;
      REEPROM();
      break;

    default:
      UART_Puts(MS5);
      HELP();
      break;
  }
}
int main(void) {
  wrapASM(Mega328P_Init);
  UARTInitPS(103U);  // start with the UART as 9600 baud, 2 stop bits, no parity
  Banner();
  while (1) {
    Command();  // infinite command loop
  }
}
