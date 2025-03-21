/*
 * pinstacking.c
 *
 * Created: 3/19/2025 2:38:53 AM
 *  Author: malon
 */ 
 #include "pinstacking.h"
 #include "portAPI.h"
 #include "termcontrol.h"
 extern void UART_Init(unsigned int);
 extern void UART_Puts(const char *str);

bool RXFlag = false;
inline void pollForUARTReceive(){
portWritePin(&PORTC,5,1);
portWritePin(&PORTC,5,0);
	if(!portReadPin(&PORTD,0)){
		RXFlag = true;
	}
	
}
void nullFunction(){
return;
}
void delayLoop(uint16_t cnt){
//cnt = 6000 is ~2.6ms with -O0
//cnt = 3000 is ~1.3ms with -O0
	for(uint16_t i =0;i<cnt;i++){
	pollForUARTReceive();
	}
}
#define UART_LOOP_DELAY_COUNT 6000
void enableUARTCallback(){
	UART_Init(103U);
	delayLoop(UART_LOOP_DELAY_COUNT);
pollForUARTReceive();
	UART_Puts(TTCSI"u");
	UART_Puts(TTCSI"0J");
	delayLoop(UART_LOOP_DELAY_COUNT);
}
void disableUARTCallback(){
	delayLoop(UART_LOOP_DELAY_COUNT);
	UART_Puts(TTCSI"s");
	UCSR0B = (0<<RXEN0)|(0<<TXEN0);
configInputPin(&PORTD,0);
	delayLoop(UART_LOOP_DELAY_COUNT);
}

const PSCallbacks animateStacking= {
	enableUARTCallback,
disableUARTCallback};
//pcint16
void enableReceiveFlag(){
configInputPin(&PORTD,0);
pollForUARTReceive();
return;
}
PSCallbacks LCDStacking = {enableReceiveFlag,nullFunction};
