/*
 * LCD.c
 *
 * Created: 3/7/2025 11:16:43 PM
 *  Author: Joe Maloney
 */
#include "LCD.h"

#include "delay.h"
#include "portAPI.h"
struct LCDConfig LCDConfig = {
    .isCursorIncrement = 1,
    .isDisplayShift = 0,
    .isDisplayOn = 1,
    .isCursorOn = 0,
    .isCursorBlinkOn = 0,
    .is8BitData = 1,
    .is2LineMode = 0,
    .is5x11Font = 0,
};
inline void LCDLatchData() {
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 1);
  delayMicroseconds(1);
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 0);
}
void LCDRegSelect(bool isData) {
  if (isData) {
    portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  } else {
    portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  }
}
void LCDCursorHome() {
  LCDWriteCommand(0x2);
  delayMicroseconds(LCD_LONG_DELAY);
}
inline void LCDZeroOutputs() {
  portWritePin(&LCD_RW_PORT, LCD_RW_PIN, 0);
  portWritePin(&LCD_LATCH_PORT, LCD_LATCH_PIN, 0);
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  portWrite(&LCD_DATA_PORT, 0x0);
}
inline void LCDWriteData(uint8_t data) {
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 1);
  portWrite(&LCD_DATA_PORT, data);
  LCDLatchData();
  delayMicroseconds(LCD_SHORT_DELAY);
  LCDZeroOutputs();
}
inline void LCDWriteCommand(uint8_t data) {
  portWritePin(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN, 0);
  portWrite(&LCD_DATA_PORT, data);
  LCDLatchData();
  delayMicroseconds(LCD_SHORT_DELAY);
  LCDZeroOutputs();
}
void LCDClear() {
  LCDWriteCommand(0x1);
  delayMicroseconds(LCD_LONG_DELAY);
}
void LCDInit() {
  // configurePins
  for (int i = 0; i <= 7; i++) {
    portConfigOutput(&LCD_DATA_PORT, i);
  }
  portConfigOutput(&LCD_LATCH_PORT, LCD_LATCH_PIN);
  portConfigOutput(&LCD_REG_SELECT_PORT, LCD_REG_SELECT_PIN);
  portConfigOutput(&LCD_RW_PORT,
                   LCD_RW_PIN);  // needs to be low for duration of LCD usage
  LCDZeroOutputs();
  // function set
  uint8_t tmp = ((1 << 5) | (LCDConfig.is8BitData << 4) |
                 (LCDConfig.is2LineMode << 3) | (LCDConfig.is5x11Font << 2));
  LCDWriteCommand(tmp);
  tmp = (1 << 3);  // Display ON/OFF
  LCDWriteCommand(tmp);
  // Entry Mode set
  tmp = ((1 << 2) | (LCDConfig.isCursorIncrement << 1) |
         LCDConfig.isDisplayShift);
  LCDWriteCommand(tmp);
  // Display ON/OFF
  tmp = ((1 << 3) | (LCDConfig.isDisplayOn << 2) | (LCDConfig.isCursorOn << 1) |
         LCDConfig.isCursorBlinkOn);
  LCDWriteCommand(tmp);
  LCDClear();
  LCDCursorHome();
}
void LCDWriteCustomChar(CustomChar *customChar, uint8_t addr) {
  addr = (1 << 6) | (addr << 3);  //"Set CGRAM address" from datasheet
  LCDWriteCommand(addr);
  for (uint8_t i = 0; i < 8; i++) {
    LCDWriteData(customChar->lines[i]);
  }
}
