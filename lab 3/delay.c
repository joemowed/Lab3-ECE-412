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
static const uint16_t minMicroseconds = 24;
static const uint16_t tuningMicroseconds = 25; // The time it takes for delayMicroseconds to operate,if the interrupt triggers instantly
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
	DELAY_OCRAH = low;
	DELAY_OCRAL = high;
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
return 1;
	if (value > 4090)
	{ //~4090 is max for 16bit timer at 16mhz
		return UINT16_MAX;
	}
	return ((value << 4) - ((tuningMicroseconds - 3) << 4)); // works because 16mhz has 1/16 microsecond period
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
// max 4090 microseconds, min 8 microseconds.  Operates with TIM1 in interrupt mode
inline void delayMicroseconds(const uint16_t microseconds)
{	if(microseconds < 3){// function call alone takes 2.31 microseconds
	return;}
	
	if(microseconds <minMicroseconds){
	for(uint16_t i = 0;i<(minMicroseconds*4);i++){
	asm("nop");
	}
	}
	stopTimer();
	setupTimerForDelay(microsecondsToPeriodCount(microseconds));
	uint8_t sreg = SREG;
	sei();
	startTimer();
	while (!waitFlagForDelay)
	{
	}
	waitFlagForDelay = false;
	SREG = sreg;
	stopTimer();
}
