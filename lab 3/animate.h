/*
 * animate.h
 *
 * Created: 3/8/2025 8:56:16 AM
 *  Author: Joe Maloney
 */

#ifndef ANIMATE_H_
#define ANIMATE_H_

#include "pinstacking.h"
#include <stdbool.h>
// buffer size in chars, 8 (or less) chars are used for spaces on the marquee
// (e.g. 20-8= 16 char string max)
#define ANIMATE_BUFFER_SIZE 120

// the total frame delay is this number * 4 (e.g. ANIMATE_DELAY =30, frame delay
// will be 30*4 = 120 milliseconds (8.3FPS)
#define ANIMATE_DELAY 30

// number of spaces before string loops back around on marquee the string is
// only used once, on the first call to animate where the initilization occurs
#define ANIMATE_SPACE_COUNT 5

/* First call to animate loads the string into the animate buffer, and draws the
 * first frame. Subsequent calls to animate will draw frames only if the time
 * calculated by "ANIMATE_DELAY" within this header has passed.  Only the first
 * call to animate loads the string, subsequent calls take the string as a
 * parameter but do not update the string within animate.*/
bool animate(const char *str,const PSCallbacks * pinStacking,PSCallbacks *LCDStacking);

#endif /* ANIMATE_H_ */
