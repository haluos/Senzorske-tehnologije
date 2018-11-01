#ifndef USART_MOJE_H
#define USART_MOJE_H

#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_conf.h>
#include <misc.h>
#include "packing.h"
#include "defines.h"
//#include "timer.h"

void usart1_init(void);
void sendData (uint8_t data);
void sendPack (uint8_t *pack, int size);
int fillBuffer (uint8_t *pack, int size);
int getRequest (uint8_t* data);
int push_Buffer (uint8_t data);
int pop_Buffer (uint8_t *data);
void DMA_SendData (int counter);
void buffer_initialization (void);
void empty_buffer (void);
void readRequest(char *request);
void USART6_Init(void);
int USART6_Dequeue(char* c);
void USART6_SendChar(char c);
int check_flag(void);

#endif
