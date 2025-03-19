/*
 * font.h
 *
 * Created: 3/8/2025 7:15:25 AM
 *  Author: Joe Maloney
 */

#ifndef FONT_H_
#define FONT_H_

#include "LCD.h"

// returns a LCD compatible CustomChar, given a ascii character code
CustomChar getFontChar(uint8_t curChar);

#endif /* FONT_H_ */
