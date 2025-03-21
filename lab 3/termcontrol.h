/*
 * termcontrol.h
 *
 * Created: 3/18/2025 10:46:02 PM
 *  Author: Joe Maloney
 */

#ifndef TERMCONTROL_H_
#define TERMCONTROL_H_

#define TTESC "\x1B"    // terminal escape sequence
#define TTCSI TTESC "[" // terminal CSI command start sequence
#define TTCR "\x0d" // terminal carriage return, sets cursor to leftmost margin
#define TTLF "\x0a" // terminal line feed, inserts newline

#endif /* TERMCONTROL_H_ */
