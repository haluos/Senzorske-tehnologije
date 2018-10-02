#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_conf.h>
#include "timer.h"
#include <usart.h>
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1

void Init_I2C_GPIO(void);
void start_trans(I2C_TypeDef* I2Cx, uint8_t address);
void start_rec(I2C_TypeDef* I2Cx, uint8_t address);
void i2c_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t read_ack(I2C_TypeDef* I2Cx);
uint8_t read_nack(I2C_TypeDef* I2Cx);
void tmp101_init (uint8_t conf);
float tmp_temp(void);
