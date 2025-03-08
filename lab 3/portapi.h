/*
* portapi.h
*
* Created: 3/7/2025 9:21:09 AM
*  Author: Joe Maloney
*/


#ifndef PORTAPI_H_
#define PORTAPI_H_
#include <stdint-gcc.h>
#include <stdbool.h>
#include "regAPI.h"
typedef struct PortStruct {
	volatile uint8_t PINx; //write only
	volatile uint8_t DDRx;
	volatile uint8_t PORTx;
} PortStruct;
typedef volatile PortStruct *const Port;
inline Port getAddrFromPort(volatile uint8_t* portRegister){
	return (Port)(portRegister-2);
}
void portConfigInput( volatile uint8_t* portaddress,uint8_t pin){
	Port port = getAddrFromPort(portaddress);
	CLEAR_BIT(port->PORTx,pin);
	CLEAR_BIT(port->DDRx,pin);
	return;
}

void portConfigOutput( volatile uint8_t* portaddress,uint8_t pin){
	Port port = getAddrFromPort(portaddress);
	CLEAR_BIT(port->PORTx,pin);
	SET_BIT(port->DDRx,pin);
	return;
}
//takes  250ns with inline (4 cycles)
inline void portTogglePin( volatile uint8_t* portaddress,uint8_t pin){
	Port port = getAddrFromPort(portaddress);
	port->PINx = (1<<pin);
	return;
}
//takes  438ns with inline (7 cycles)
inline void portWritePin( volatile uint8_t* portaddress,uint8_t pin,bool value){
	Port port = getAddrFromPort(portaddress);
	SET_BIT_VALUE(port->PORTx,pin,value);
	return;
}

inline uint8_t portRead( volatile uint8_t* portaddress){
	Port port = getAddrFromPort(portaddress);
	return port->PORTx;
}
inline bool portReadPin( volatile uint8_t* portaddress,uint8_t pin){
	Port port = getAddrFromPort(portaddress);
	return READ_BIT(port->PORTx,pin);
}
inline void portWrite( volatile uint8_t* portaddress,uint8_t value){
	Port port = getAddrFromPort(portaddress);
	port->PORTx = value;
	return;
}
#endif /* PORTAPI_H_ */