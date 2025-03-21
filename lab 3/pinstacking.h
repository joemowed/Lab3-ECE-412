/*
 * pinstacking.h
 *
 * Created: 3/18/2025 9:35:07 PM
 *  Author: Joe Maloney
 */

#ifndef PINSTACKING_H_
#define PINSTACKING_H_

#include <stdbool.h>

/*Sets the delay that occurs before starting and after stopping the UART as it
 * is enabled/disabled by the 'animateStacking' functions.*/
#define UART_LOOP_DELAY_COUNT 6000

// typedef for pinstacking functions to be passed to the LCD and animate class
typedef struct PinStackingCallbackStruct {
    void (*enable)(void);
    void (*disable)(void);
} PSCallbacks;

/* Global flag indicating that the UART RX pin used by the mEDBG was low when it
 * was polled.  This value is set to true when the RX pin is polled as low.  It
 * must be reset to false by the application when it is handled. */
bool RXFlag;

// polls the mEDBG RX pin and sets the RXFlag if the pin is low
void pollForUARTReceive();

// callbacks used by animate for pinstacking
const PSCallbacks animateStacking;

// callbacks used by the LCD class for pinstacking
PSCallbacks LCDStacking;

#endif /* PINSTACKING_H_ */
