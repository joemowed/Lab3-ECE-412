/*
 * animate.h
 *
 * Created: 3/8/2025 8:56:16 AM
 *  Author: malon
 */

#ifndef ANIMATE_H_
#define ANIMATE_H_
#include "font.h"
#define ANIMATE_BUFFER_SIZE 90 // buffer size in chars, 8 (or less) chars are used for spaces on the marquee (e.g. 20-8= 16 char string max) Max value: 30
#define ANIMATE_DELAY 30       // the total frame delay is this number * 4 (e.g. ANIMATE_DELAY =5, frame delay will be 5*4 = 20 milliseconds
#define ANIMATE_SPACE_COUNT 5  // number of spaces before string loops back around on marquee
// the string is only used once, on the first call to animate where the inilization occurs.
void animate(const uint8_t *str);

#endif /* ANIMATE_H_ */