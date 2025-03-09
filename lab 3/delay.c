/*
* delay.c
*
* Created: 3/8/2025 12:23:33 AM
*  Author: Joe Maloney
*/
#include "delay.h"
#include <avr/interrupt.h>
#include <stdbool.h>
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
inline void resetCount(volatile uint8_t*CTRLB){
	volatile uint8_t* TCNTL = (CTRLB+4);
	writeTimerRegisterWord(TCNTL,0x0);
}
inline void setupTimerForDelay(volatile uint8_t* CTRLB,volatile uint8_t*timerISRMSK,uint16_t compare){
	volatile uint8_t* CTRLA = (CTRLB-1);
	*CTRLA = 0x0; //reset state
	resetCount(CTRLB);
	volatile uint8_t* OCRAL = (CTRLA+8);
	writeTimerRegisterWord(OCRAL,compare);
	*timerISRMSK = (1<<OCIE1A);//enable only the a compare interrupt
}
inline uint16_t microsecondsToPeriodCount(uint16_t value){
	const uint16_t minMicroseconds = 8; //The time it takes for delayMicroseconds to operate,if the interrupt triggers instantly
	//The below codes makes the timer sometimes run to the full counter duration (4090us)
	//if(value < minMicroseconds){
	//return 1U;
	//}
	if(value > 4090){ //~4090 is max for 16bit timer at 16mhz
		return UINT16_MAX;
	}
	return ((value<<4) - ((minMicroseconds-3)<<4)); //works because 16mhz has 1/16 microsecond period
}
inline void startTimer(volatile uint8_t*CTRLB){
	*CTRLB = ((0x1<<WGM12)|(0x1)); // 0x1 sets clk src to cpu clk and starts timer.  CTC mode auto-resets the count on compareA match
}
inline void stopTimer(volatile uint8_t*CTRLB,volatile uint8_t*timerISRMSK){
	*CTRLB = 0x0;
	*timerISRMSK = 0;
}
//max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt mode
void delayMicroseconds(uint16_t microseconds){
	stopTimer(&DELAY_CTRLB,&DELAY_ISRMSK);
	setupTimerForDelay(&DELAY_CTRLB,&DELAY_ISRMSK,microsecondsToPeriodCount(microseconds));
	uint8_t sreg = SREG;
	sei();
	startTimer(&DELAY_CTRLB);
	while(!waitFlagForDelay){}
	waitFlagForDelay = false;
	SREG = sreg;
	stopTimer(&DELAY_CTRLB,&DELAY_ISRMSK);
}
