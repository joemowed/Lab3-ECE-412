/*
 * timerAPI.h
 *
 * Created: 3/7/2025 12:47:00 PM
 *  Author: Joe Maloney
 */

#ifndef TIMERAPI_H_
#define TIMERAPI_H_
#include <avr/common.h>
#include <stdbool.h>
#include <stdint-gcc.h>
// select the timer to use here.  Works with TIM1,TIM3, and TIM4
#define DELAY_TIMER_NUMBER 1
#if DELAY_TIMER_NUMBER == 1
// non-relative addressing (e.g. "#define DELAY_CTRLA TCCR1A") makes the min
// timer time 23.9us
#define DELAY_CTRLA TCCR1A
#define DELAY_CTRLB TCCR1B
#define DELAY_TCNTH TCNT1H
#define DELAY_TCNTL TCNT1L
#define DELAY_OCRAH OCR1AH
#define DELAY_OCRAL OCR1AL
#define DELAY_ISRMSK TIMSK1
#define DELAY_ISR_HANDLE TIMER1_COMPA_vect
#elif DELAY_TIMER_NUMBER == 3
#define DELAY_CTRLA TCCR3A
#define DELAY_CTRLB TCCR3B
#define DELAY_TCNTH TCNT3H
#define DELAY_TCNTL TCNT3L
#define DELAY_OCRAH OCR3AH
#define DELAY_OCRAL OCR3AL
#define DELAY_ISRMSK TIMSK3
#define DELAY_ISR_HANDLE TIMER3_COMPA_vect
#elif DELAY_TIMER_NUMBER == 4
#define DELAY_CTRLA TCCR4A
#define DELAY_CTRLB TCCR4B
#define DELAY_TCNTH TCNT4H
#define DELAY_TCNTL TCNT4L
#define DELAY_OCRAH OCR4AH
#define DELAY_OCRAL OCR4AL
#define DELAY_ISRMSK TIMSK4
#define DELAY_ISR_HANDLE TIMER4_COMPA_vect
#else
#define DELAY_CTRLA TCCR1A
// #define DELAY_CTRLB (*(&(DELAY_CTRLA) +1))
// #define DELAY_ISRMSK (*(&(DELAY_CTRLA) -11))
// #define DELAY_TCNTH  (*(&(DELAY_CTRLA) +5))
// #define DELAY_TCNTL (*(&(DELAY_CTRLA) +4))
// #define DELAY_OCRAH (*(&(DELAY_CTRLA) +9))
// #define DELAY_OCRAL (*(&(DELAY_CTRLA) +8))
// #define DELAY_ISR_HANDLE TIMER1_COMPA_vect
#error selected timer not a valid timer for delayMicroseconds
#endif
// max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt
// mode
void delayMicroseconds(uint16_t microseconds);
#endif /* TIMERAPI_H_ */