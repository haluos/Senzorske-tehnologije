#include <stm32f4xx.h> 				// common stuff
#include <stm32f4xx_rcc.h> 		// reset anc clocking
#include <stm32f4xx_gpio.h>	  // gpio control
#include "stm32f4xx_tim.h"
#include "timer.h"

void Init_PWM1(void);
void Init_PWM2(void);
void reset_pins(void);
void Start_PWM1 (int duty_cycle);
void Start_PWM2 (int duty_cycle);
void All_OFF(void);
