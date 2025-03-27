
/*
 * regrestore.s
 *
 * Created: 3/19/2025 12:39:52 AM
 *  Author: malon
 */ 
.section ".data"					;Make room in RAM for storing the 32 8-bit
REGSAVE_R0: .byte 1                 ;values of the registers to be saved/restored
REGSAVE_R1: .byte 1
REGSAVE_R2: .byte 1
REGSAVE_R3: .byte 1
REGSAVE_R4: .byte 1
REGSAVE_R5: .byte 1
REGSAVE_R6: .byte 1
REGSAVE_R7: .byte 1
REGSAVE_R8: .byte 1
REGSAVE_R9: .byte 1
REGSAVE_R10: .byte 1
REGSAVE_R11: .byte 1
REGSAVE_R12: .byte 1
REGSAVE_R13: .byte 1
REGSAVE_R14: .byte 1
REGSAVE_R15: .byte 1
REGSAVE_R16: .byte 1
REGSAVE_R17: .byte 1
REGSAVE_R18: .byte 1
REGSAVE_R19: .byte 1
REGSAVE_R20: .byte 1
REGSAVE_R21: .byte 1
REGSAVE_R22: .byte 1
REGSAVE_R23: .byte 1
REGSAVE_R24: .byte 1
REGSAVE_R25: .byte 1
REGSAVE_R26: .byte 1
REGSAVE_R27: .byte 1
REGSAVE_R28: .byte 1
REGSAVE_R29: .byte 1
REGSAVE_R30: .byte 1
REGSAVE_R31: .byte 1

.section ".text"					
.global saveRegs                   ; saves all cpu registers to RAM
saveRegs: sts REGSAVE_R0, r0       ; to be restored later
 sts REGSAVE_R1, r1
 sts REGSAVE_R2, r2
 sts REGSAVE_R3, r3
 sts REGSAVE_R4, r4
 sts REGSAVE_R5, r5
 sts REGSAVE_R6, r6
 sts REGSAVE_R7, r7
 sts REGSAVE_R8, r8
 sts REGSAVE_R9, r9
 sts REGSAVE_R10, r10
 sts REGSAVE_R11, r11
 sts REGSAVE_R12, r12
 sts REGSAVE_R13, r13
 sts REGSAVE_R14, r14
 sts REGSAVE_R15, r15
 sts REGSAVE_R16, r16
 sts REGSAVE_R17, r17
 sts REGSAVE_R18, r18
 sts REGSAVE_R19, r19
 sts REGSAVE_R20, r20
 sts REGSAVE_R21, r21
 sts REGSAVE_R22, r22
 sts REGSAVE_R23, r23
 sts REGSAVE_R24, r24
 sts REGSAVE_R25, r25
 sts REGSAVE_R26, r26
 sts REGSAVE_R27, r27
 sts REGSAVE_R28, r28
 sts REGSAVE_R29, r29
 sts REGSAVE_R30, r30
 sts REGSAVE_R31, r31
 sts REGSAVE_R31, r31
 ret
 
 .global restoreRegs             ;restores all cpu registers that were
 restoreRegs: lds r0, REGSAVE_R0 ; saved to RAM with 'saveRegs()'
  lds r1, REGSAVE_R1
  lds r2, REGSAVE_R2
  lds r3, REGSAVE_R3
  lds r4, REGSAVE_R4
  lds r5, REGSAVE_R5
  lds r6, REGSAVE_R6
  lds r7, REGSAVE_R7
  lds r8, REGSAVE_R8
  lds r9, REGSAVE_R9
  lds r10, REGSAVE_R10
  lds r11, REGSAVE_R11
  lds r12, REGSAVE_R12
  lds r13, REGSAVE_R13
  lds r14, REGSAVE_R14
  lds r15, REGSAVE_R15
  lds r16, REGSAVE_R16
  lds r17, REGSAVE_R17
  lds r18, REGSAVE_R18
  lds r19, REGSAVE_R19
  lds r20, REGSAVE_R20
  lds r21, REGSAVE_R21
  lds r22, REGSAVE_R22
  lds r23, REGSAVE_R23
  lds r24, REGSAVE_R24
  lds r25, REGSAVE_R25
  lds r26, REGSAVE_R26
  lds r27, REGSAVE_R27
  lds r28, REGSAVE_R28
  lds r29, REGSAVE_R29
  lds r30, REGSAVE_R30
  lds r31, REGSAVE_R31
  lds r31, REGSAVE_R31
  ret
