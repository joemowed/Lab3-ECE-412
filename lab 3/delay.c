/*
* delay.c
*
* Created: 3/8/2025 12:23:33 AM
*  Author: Joe Maloney
*/
#include "delay.h"
#include <avr/interrupt.h>
#include <stdbool.h>
static volatile bool waitFlagForDelay = false;
const uint16_t tuningMicroseconds = 9; // The time it takes for delayMicroseconds to operate,if the interrupt triggers instantly
ISR(DELAY_ISR_HANDLE, ISR_BLOCK)
{
	waitFlagForDelay = true;
	DELAY_CTRLB = 0x0;
}
// required by datasheet to write high byte first
inline void writeTimerCompare(uint16_t value)
{
	uint8_t sreg = SREG;
	cli();
	const volatile uint8_t low = (uint8_t)(value & 0xFF);
	const volatile uint8_t high = (uint8_t)(value >> 8);
	DELAY_OCRAH = high;
	DELAY_OCRAL = low;
	SREG = sreg;
}
inline void resetCount()
{
	DELAY_TCNTH = 0x0;
	DELAY_TCNTL = 0x0;
}
inline void setupTimerForDelay(uint16_t compare)
{
	DELAY_CTRLA = 0x0; // reset state
	resetCount();
	writeTimerCompare(compare);
	DELAY_ISRMSK = (1 << OCIE1A); // enable only the a compare interrupt
}
inline uint16_t microsecondsToPeriodCount(uint16_t value)
{
	if (value > 4090)
	{ //~4090 is max for 16bit timer at 16mhz
		return UINT16_MAX;
	}
	return ((value << 4) - ((tuningMicroseconds) << 4)); // works because 16mhz has 1/16 microsecond period
}
inline void startTimer()
{
	DELAY_CTRLB = ((0x1 << WGM12) | (0x1)); // 0x1 sets clk src to cpu clk and starts timer.  CTC mode auto-resets the count on compareA match
}
inline void stopTimer()
{
	DELAY_CTRLB = 0x0;
	DELAY_ISRMSK =0x0;
}
void showMeAddrs(){
	volatile uint8_t* volatile v1 = &DELAY_CTRLA;
	volatile uint8_t* volatile v2 = &DELAY_CTRLB ;
	volatile uint8_t* volatile v3 = &DELAY_ISRMSK ;
	volatile uint8_t* volatile vk = &DELAY_TCNTH  ;
	volatile uint8_t* volatile v8 = &DELAY_TCNTL ;
	volatile uint8_t* volatile v9 = &DELAY_OCRAH ;
	volatile uint8_t* volatile v0 = &DELAY_OCRAL ;
	v1++;
	v2++;
	v3++;
	vk++;
	v9++;
	v8++;
	v0++;
	volatile uint8_t i =0;
	i++;
	return;
}
#include "portAPI.h"
// max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt mode
//timerinit -inline call to delaymicroseconds takes 2.0us, non-inline with return value 10us
uint8_t timerInit(const uint16_t microseconds){
	
	stopTimer();//1.12us
	const uint16_t count = microsecondsToPeriodCount(microseconds); //0.313us
	setupTimerForDelay(count);//1.8us
	const uint8_t sreg = SREG;
	sei(); 
	startTimer();
	return sreg;
}
 void approxLoop(const uint16_t microseconds){
	for(volatile uint8_t i =0;i<microseconds-3;i++){}
}
void delayWrapper(const uint16_t microseconds){
	uint8_t sreg = timerInit(microseconds);
	while (!waitFlagForDelay)
	{
	}
	waitFlagForDelay = false;
	SREG = sreg;
	stopTimer();
}
void delayMicroseconds(const uint16_t microseconds)
{	if(microseconds <= 2){// function call alone takes 938 microseconds
	return;
}else
{
	if(microseconds <tuningMicroseconds){
		approxLoop(microseconds);
	}
	else{
		delayWrapper(microseconds);
	}
}
}
