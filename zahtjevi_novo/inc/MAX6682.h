#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_conf.h>
#include "timer.h"
#include <usart.h>
#include <stm32f4xx_spi.h>

void SPI1_Init (void);
uint16_t max6682temp(float *temp);
