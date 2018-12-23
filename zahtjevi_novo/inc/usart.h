/* usart.h */
#ifndef USART_H
#define USART_H

#include <stm32f4xx.h> 				// common stuff
#include <stm32f4xx_rcc.h> 		// reset anc clocking
#include <stm32f4xx_gpio.h>	  // gpio control
#include <stm32f4xx_usart.h> 	// USART
#include <stdlib.h>
#include "defines.h"

// #define BUFSIZE 								16
// #define BAUDRATE							 	115200

// void USART1_Init(void); 			// init USART1 peripheral
// void USART1_SendChar(char c); // blocking send character
// int USART1_Dequeue(char* c); 	// pop character from receive FIFO

void USART6_Init(void); 			// init USART1 peripheral
void USART6_SendChar(char c); // blocking send character
int USART6_Dequeue(char* c); 	// pop character from receive FIFO
// int readTempRequest(void);
int readRequest(void);
// void toggle_init(void);
// void getRequest(void);
// int get_duty (int direction);
// int push_Buffer (uint8_t data);
int pop_Buffer (uint8_t *data);
int fillBuffer (uint8_t *pack, int size);
void buffer_initialization (void);
void empty_buffer (void);
int continue_measure (void);

#endif
