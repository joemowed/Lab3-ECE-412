/*
 * regAPI.c
 *
 * Created: 3/19/2025 12:20:31 AM
 *  Author: malon
 */
#include "regAPI.h"
#include <stdint-gcc.h>
// saves all general purpose registers
register uint8_t *r0 asm("r0");
register uint8_t r1 asm("r1");
register uint8_t r2 asm("r2");
register uint8_t r3 asm("r3");
register uint8_t r4 asm("r4");
register uint8_t r5 asm("r5");
register uint8_t r6 asm("r6");
register uint8_t r7 asm("r7");
register uint8_t r8 asm("r8");
register uint8_t r9 asm("r9");
register uint8_t r10 asm("r10");
register uint8_t r11 asm("r11");
register uint8_t r12 asm("r12");
register uint8_t r13 asm("r13");
register uint8_t r14 asm("r14");
register uint8_t r15 asm("r15");
register uint8_t r16 asm("r16");
register uint8_t r17 asm("r17");
register uint8_t r18 asm("r18");
register uint8_t r19 asm("r19");
register uint8_t r20 asm("r20");
register uint8_t r21 asm("r21");
register uint8_t r22 asm("r22");
register uint8_t r23 asm("r23");
register uint8_t r24 asm("r24");
register uint8_t r25 asm("r25");
register uint8_t r26 asm("r26");
register uint8_t r27 asm("r27");
register uint8_t r28 asm("r28");
register uint8_t r29 asm("r29");
register uint8_t r30 asm("r30");
register uint8_t r31 asm("r31");
static uint8_t savedRegs[32];
void saveRegs() {
  savedRegs[0] = r0;
  savedRegs[1] = r1;
  savedRegs[2] = r2;
  savedRegs[3] = r3;
  savedRegs[4] = r4;
  savedRegs[5] = r5;
  savedRegs[6] = r6;
  savedRegs[7] = r7;
  savedRegs[8] = r8;
  savedRegs[9] = r9;
  savedRegs[10] = r10;
  savedRegs[11] = r11;
  savedRegs[12] = r12;
  savedRegs[13] = r13;
  savedRegs[14] = r14;
  savedRegs[15] = r15;
  savedRegs[16] = r16;
  savedRegs[17] = r17;
  savedRegs[18] = r18;
  savedRegs[19] = r19;
  savedRegs[20] = r20;
  savedRegs[21] = r21;
  savedRegs[22] = r22;
  savedRegs[23] = r23;
  savedRegs[24] = r24;
  savedRegs[25] = r25;
  savedRegs[26] = r26;
  savedRegs[27] = r27;
  savedRegs[28] = r28;
  savedRegs[29] = r29;
  savedRegs[30] = r30;
  savedRegs[31] = r31;
}
// restores all general purpose registers
void restoreRegs() {
  r0 = savedRegs[0];
  r1 = savedRegs[1];
  r2 = savedRegs[2];
  r3 = savedRegs[3];
  r4 = savedRegs[4];
  r5 = savedRegs[5];
  r6 = savedRegs[6];
  r7 = savedRegs[7];
  r8 = savedRegs[8];
  r9 = savedRegs[9];
  r10 = savedRegs[10];
  r11 = savedRegs[11];
  r12 = savedRegs[12];
  r13 = savedRegs[13];
  r14 = savedRegs[14];
  r15 = savedRegs[15];
  r16 = savedRegs[16];
  r17 = savedRegs[17];
  r18 = savedRegs[18];
  r19 = savedRegs[19];
  r20 = savedRegs[20];
  r21 = savedRegs[21];
  r22 = savedRegs[22];
  r23 = savedRegs[23];
  r24 = savedRegs[24];
  r25 = savedRegs[25];
  r26 = savedRegs[26];
  r27 = savedRegs[27];
  r28 = savedRegs[28];
  r29 = savedRegs[29];
  r30 = savedRegs[30];
  r31 = savedRegs[31];
}
