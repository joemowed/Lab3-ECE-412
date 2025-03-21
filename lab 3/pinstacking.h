/*
 * pinstacking.h
 *
 * Created: 3/18/2025 9:35:07 PM
 *  Author: Joe Maloney
 */

#ifndef PINSTACKING_H_
#define PINSTACKING_H_
#define UART_LOOP_DELAY_COUNT 6000
#include <stdbool.h>
typedef struct PinStackingCallbackStruct {
  void (*enable)(void);
  void (*disable)(void);
} PSCallbacks;
bool RXFlag;
void pollForUARTReceive();
const PSCallbacks animateStacking;
PSCallbacks LCDStacking;
#endif /* PINSTACKING_H_ */