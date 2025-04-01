/*
 * pinstacking.c
 *
 * Created: 3/19/2025 2:38:53 AM
 *  Author: malon
 */

#include "pinstacking.h"
#include "portAPI.h"
#include "termcontrol.h"

extern void UARTInitPS(unsigned int);    // defined in main.c
extern void UART_Puts(const char *str); // defined in main.c

bool RXFlag = false; // global flag

// sets the above global flag if the mEDGB RX pin is low when called
inline void pollForUARTReceive() {
    if (!portReadPin(&PORTD, 0)) { // read the pin
        RXFlag = true;             // set the flag
    }
}

// callback that does nothing, used for the enable side of LCDStacking
void nullFunction() { return; }

/* Loop based delay, used within the pinstacking callbacks because the timer
 * based delay is already being used by animate.  Also calls the
 * 'pollForUARTReceive()' function each iteration.*/
void delayLoop(uint16_t cnt) {
    // cnt = 6000 is ~2.6ms with -O0
    // cnt = 3000 is ~1.3ms with -O0
    for (uint16_t i = 0; i < cnt; i++) {
        pollForUARTReceive(); // poll the UART RX pin
    }
}

/*  Enables the UART, and restores previously saved cursor position.  After the
 * cursor is restored, the terminal is cleared from the cursor to the bottom of
 * the screen.  This deletes any garbage data sent to the terminal by the writes
 * to the LCD. */
void enableUARTCallback() {
    UARTInitPS(103U); // enable UART
    // wait for any previous data sent to the terminal to be read by the
    // terminal
    delayLoop(UART_LOOP_DELAY_COUNT);
	//this is the largest gap in polling, at 5.83ms between the delayLoop() calls
    // restore the cursor to the saved position
    UART_Puts(TTCSI "u");
    // delete from the cursor to the bottom of terminal
    UART_Puts(TTCSI "0J");
    // wait for UART pins to return to waiting for data state
    delayLoop(UART_LOOP_DELAY_COUNT);
}

/* Disables the UART after sending the terminal command to save the cursor
 * position*/
void disableUARTCallback() {
    // wait for UART pins to return to waiting for data state
    delayLoop(UART_LOOP_DELAY_COUNT);
    // send terminal command to save cursor position
    UART_Puts(TTCSI "s");
    // disable the UART
    UCSR0B = (0 << RXEN0) | (0 << TXEN0);
    // enable the pull-up on the RX pin, because the pin is no longer controlled
    // by the UART peripherial
    configInputPin(&PORTD, 0);
    // wait for the UART pins to free from the UART
    delayLoop(UART_LOOP_DELAY_COUNT);
}

// callbacks used by animate
const PSCallbacks animateStacking = {enableUARTCallback, disableUARTCallback};

// polls the UART RX pin each time data is written to the LCD
void enableReceiveFlag() {
    // enable the pull-up
    configInputPin(&PORTD, 0);
    // poll the RX pin
    pollForUARTReceive();
    return;
}

void UARTInitPS( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
		/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	/*	Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);

}

// callbacks used by the LCD class
PSCallbacks LCDStacking = {enableReceiveFlag, nullFunction};
