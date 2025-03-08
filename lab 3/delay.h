/*
* timerAPI.h
*
* Created: 3/7/2025 12:47:00 PM
*  Author: Joe Maloney
*/


#ifndef TIMERAPI_H_
#define TIMERAPI_H_
#include <stdint-gcc.h>
#include <avr/common.h>
#include <stdbool.h>
//select the timer to use here.  Works with TIM1,TIM3, and TIM4
#define DELAY_TIMER_NUMBER 1
#if DELAY_TIMER_NUMBER == 1
#define DELAY_CTRLB TCCR1B
#define DELAY_ISRMSK TIMSK1
#define DELAY_ISR_HANDLE TIMER1_COMPA_vect
#elif DELAY_TIMER_NUMBER == 3
#define DELAY_CTRLB TCCR3B
#define DELAY_ISRMSK TIMSK3
#define DELAY_ISR_HANDLE TIMER3_COMPA_vect
#elif DELAY_TIMER_NUMBER == 4
#define DELAY_CTRLB TCCR4B
#define DELAY_ISRMSK TIMSK4
#define DELAY_ISR_HANDLE TIMER4_COMPA_vect
#else
#error selected timer not a valid timer for delayMicroseconds
#endif
//max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt mode
void delayMicroseconds(uint16_t microseconds);
#endif /* TIMERAPI_H_ */