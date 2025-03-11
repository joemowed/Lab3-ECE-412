/*
 * profiling.h
 *
 * Created: 3/11/2025 1:53:00 AM
 *  Author: Joe Maloney
 */ 


#ifndef PROFILING_H_
#define PROFILING_H_


void __attribute__((constructor)) paintStack(void){
	extern uint8_t  __heap_start	;
	volatile uint8_t* tmp = &__heap_start;
	while(tmp <=(uint8_t*)0x8F9){
		*tmp = 0xDE;
		tmp++;
		*tmp = 0xAD;
		tmp++;
	}
}


#endif /* PROFILING_H_ */