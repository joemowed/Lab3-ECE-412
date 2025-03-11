/*
 * animate.h
 *
 * Created: 3/8/2025 8:56:16 AM
 *  Author: malon
 */

#ifndef ANIMATE_H_
#define ANIMATE_H_
#define ANIMATE_BUFFER_SIZE \
  120  // buffer size in chars, 8 (or less) chars are used for spaces on the
      // marquee (e.g. 20-8= 16 char string max) Max value: 30
#define ANIMATE_DELAY \
  30  // the total frame delay is this number * 4 (e.g. ANIMATE_DELAY =30, frame
      // delay will be 30*4 = 120 milliseconds (8.3FPS)
#define ANIMATE_SPACE_COUNT \
  5  // number of spaces before string loops back around on marquee
// the string is only used once, on the first call to animate where the
// inilization occurs.
void animate(const char* str);

#endif /* ANIMATE_H_ */