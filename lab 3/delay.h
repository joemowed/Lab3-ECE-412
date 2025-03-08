/*
* timerAPI.h
*
* Created: 3/7/2025 12:47:00 PM
*  Author: Joe Maloney
*/


#ifndef TIMERAPI_H_
#define TIMERAPI_H_
#include <stdint-gcc.h>
#include <avr/interrupt.h>
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
#error selected timer not a valid timer for delaymicroseconds
#endif
volatile bool waitFlagForDelay = false;
ISR(DELAY_ISR_HANDLE,ISR_BLOCK){
	waitFlagForDelay = true;
	DELAY_CTRLB = 0x0;
}
//required by datasheet to write high byte first
inline void writeTimerRegisterWord(volatile uint8_t*lowByte, uint16_t value){
	uint8_t sreg =SREG;
	cli();
	volatile uint8_t*const highByte = (lowByte +1);
	*highByte = (uint8_t)(value >>8);
	*lowByte = (uint8_t)(value & 0xFF);
	SREG = sreg;
}
inline void setupTimerForDelay(volatile uint8_t* CTRLB,volatile uint8_t*timerISRMSK,uint16_t compare){
	*CTRLB = 0x0; //disable the timer if it isn't already
	volatile uint8_t* CTRLA = (CTRLB-1);
	*CTRLA = 0x0; //reset state
	volatile uint8_t* OCRAL = (CTRLA+8);
	writeTimerRegisterWord(OCRAL,compare);
	*timerISRMSK = (1<<OCIE1A);//enable only the a compare interrupt
}
inline uint16_t microsecondsToPeriodCount(uint16_t value){
	const uint16_t minMicroseconds = 8; //The time it takes for delayMicroseconds to operate,if the interrupt triggers instantly
	if(value < minMicroseconds){
		return 1;
	}
	if(value > 4090){ //~4090 is max for 16bit timer at 16mhz
		return UINT16_MAX;
	}
	return ((value<<4) - ((minMicroseconds-3)<<4)); //works because 16mhz has 1/16 microsecond period
}
inline void startTimer(volatile uint8_t*CTRLB){
	*CTRLB = ((0x1<<WGM12)|(0x1)); // 0x1 sets clk src to cpu clk and starts timer.  CTC mode auto-resets the count on compareA match
}
inline void stopTimer(volatile uint8_t*CTRLB){
	*CTRLB = 0x0;
}
//max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt mode
void delayMicroseconds(uint16_t microseconds){
	stopTimer(&DELAY_CTRLB);
	setupTimerForDelay(&DELAY_CTRLB,&DELAY_ISRMSK,microsecondsToPeriodCount(microseconds));
	uint8_t sreg = SREG;
	sei();
	startTimer(&DELAY_CTRLB);
	while(!waitFlagForDelay){}
	waitFlagForDelay = false;
	SREG = sreg;
	stopTimer(&DELAY_CTRLB);
}
#endif /* TIMERAPI_H_ */