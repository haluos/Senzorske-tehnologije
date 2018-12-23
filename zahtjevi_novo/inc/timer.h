#ifndef TIMER_H
#define TIMER_H

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <usart.h>

void timer2_init (void);
uint32_t timer2_get_millisec (void);
void timer2_wait_millisec (uint32_t ms);
int timer_flag (void);
void reset_ds (void);
void reset_tmp (void);
int check_ds (void);
int check_tmp (void);
#endif
