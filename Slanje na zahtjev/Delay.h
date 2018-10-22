#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f4xx.h"
#include "stdint.h"
#include "ctype.h"

void SysTick_Init(void);
void TimeTick_Decrement(void);
void delay_nus(uint32_t n);
void delay_1ms(void);
void delay_nms(uint32_t n);

#endif

