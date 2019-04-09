#include <stm32f4xx.h> // common stuff
#include <stm32f4xx_gpio.h> // gpio control
#include <stm32f4xx_rcc.h> // reset anc clocking
#include <usart.h>
#include "timer.h"
#include "stm32f4xx_adc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

uint16_t NICO_temp (float *temp);
void ADC2_Init (void);
