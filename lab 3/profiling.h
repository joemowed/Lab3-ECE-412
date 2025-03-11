/*
 * profiling.h
 *
 * Created: 3/11/2025 1:53:00 AM
 *  Author: Joe Maloney
 */

#ifndef PROFILING_H_
#define PROFILING_H_

// writes "0xDEAD" pattern to the stack, so that the stack usage level can be
// determined by looking for pattern in memory inspector
void __attribute__((constructor)) paintStack(void) { // run before main,
                                                     // called automatically
    extern uint8_t __heap_start;                     // provided by linker
    volatile uint8_t *tmp = &__heap_start;
    while (tmp <= (uint8_t *)0x8F9) {
        *tmp = 0xDE;
        tmp++;
        *tmp = 0xAD;
        tmp++;
    }
}

#endif /* PROFILING_H_ */
