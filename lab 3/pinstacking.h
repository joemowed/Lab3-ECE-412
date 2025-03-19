/*
* pinstacking.h
*
* Created: 3/18/2025 9:35:07 PM
*  Author: malon
*/


#ifndef PINSTACKING_H_
#define PINSTACKING_H_


typedef struct PinStackingCallbackStruct{
	void(*enable)(void);
	void(*disable)(void);
} PSCallbacks;


#endif /* PINSTACKING_H_ */