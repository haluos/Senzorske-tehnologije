#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include "timer.h"
#include <string.h>
#include "usart.h"
#include <Delay.h>



#define  ONEWIRE_PIN_NUM                1
#define  ONEWIRE_PORT                   GPIOC

#define  ONEWIRE_CONFIG_OUTPUT          ONEWIRE_PORT->MODER|=(GPIO_Mode_OUT<<(ONEWIRE_PIN_NUM*2))
#define  ONEWIRE_CONFIG_INPUT           ONEWIRE_PORT->MODER &=~(GPIO_MODER_MODER0_0<<(ONEWIRE_PIN_NUM*2))


void gpio_init(void);
uint8_t reset(void);
uint8_t read(void);
void write(uint8_t val);
uint16_t  DS_temp(float *temperatura);
