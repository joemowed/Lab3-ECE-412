/*
 * pinstacking.c
 *
 * Created: 3/19/2025 2:38:53 AM
 *  Author: malon
 */ 
 #include "pinstacking.h"
 #include "portAPI.h"
 #include "termcontrol.h"
#include <avr/interrupt.h>
bool receiveFlag = false;
ISR(PCINT0_vect, ISR_BLOCK) {
receiveFlag = true;
}

