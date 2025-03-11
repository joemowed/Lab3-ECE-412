/*
 * regAPI.h
 *
 * Created: 3/7/2025 11:47:43 AM
 *  Author: Joe Maloney
 */

#ifndef REGAPI_H_
#define REGAPI_H_

#define SET_BIT(reg, bitPos) ((reg) |= (1 << (bitPos)))
#define SET_BIT_VALUE(reg, bitPos, value) \
  ((reg) ^= (((-value) ^ (reg)) & (1 << (bitPos))))
#define READ_BIT(reg, bitPos) ((reg) & (1 << (bitPos)))
#define CLEAR_BIT(reg, bitPos) ((reg) &= (~(1 << (bitPos))))

#endif /* REGAPI_H_ */