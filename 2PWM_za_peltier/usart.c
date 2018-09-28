/* usart.c */
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "PWM.h"

#define PRINTF(a, b) {char string[100]; sprintf(string, a, b); int i; for(i=0; i<strlen(b); ++i) USART1_SendChar(string[i]);};
int temp=0;	

// RX FIFO buffer
char RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;
// TX state flag
uint8_t TxReady;

// init USART1
void USART1_Init(void)
{
	GPIO_InitTypeDef 							GPIO_InitStruct;
	USART_InitTypeDef 						USART_InitStruct;
	NVIC_InitTypeDef 							NVIC_InitStructure;
	
	// enable peripheral clocks (note: different bus interfaces for each peripheral!)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
		// map port B pins for alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 	// Pins 6 (TX) and 7 (RX) will be used for USART1
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 						// GPIO pins defined as alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;			  // I/O pins speed (signal rise time)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 					// push-pull output
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 						// activates pullup resistors
	GPIO_Init(GPIOB, &GPIO_InitStruct); 									// set chosen pins
	
	// set alternate function to USART1 (from multiple possible alternate function choices)
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); // pins will automatically be assignedto TX/RX - refer to datasheet to see AF mappings
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	
	// use USART_InitStruct to config USART1 peripheral
	USART_InitStruct.USART_BaudRate = BAUDRATE; 							// set baudrate from define
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 8 data bits
	USART_InitStruct.USART_StopBits = USART_StopBits_1; 			// 1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; 					// no parity check
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // no HW control flow
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enable both character transmit and receive
	USART_Init(USART1, &USART_InitStruct); 												// set USART1 peripheral

	// set interrupt triggers for USART1 ISR (but do not enable USART1 interrupts yet)
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// should be disbled
	USART_ITConfig(USART1, USART_IT_TC, ENABLE); // transmission completed event (for reseting TxReady flag)
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // character received (to trigger buffering of new character)

	TxReady = 1; 														// USART1 is ready to transmit
	RX_BUFFER_HEAD = 0; RX_BUFFER_TAIL = 0; // clear rx buffer

	// prepare NVIC to receive USART1 IRQs
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 					// configure USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// max. priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 					// max. priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// enable USART1 interrupt in NVIC
	NVIC_Init(&NVIC_InitStructure);															// set NVIC for USART1 IRQ
	
	// enables USART1 interrupt generation
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
	static char rx_char;
	static char rx_head;
	// RX event
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		rx_char = USART_ReceiveData(USART1);
		// check for buffer overrun:
		rx_head = RX_BUFFER_HEAD + 1;
		if (rx_head == BUFSIZE) rx_head = 0;
		if (rx_head != RX_BUFFER_TAIL)
		{
		// adding new char will not cause buffer overrun:
		RX_BUFFER[RX_BUFFER_HEAD] = rx_char;
		RX_BUFFER_HEAD = rx_head; // update head
		}
	}

	// TX event
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{	
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	TxReady = 1;
	}
}

void USART1_SendChar(char c)
{
	while(!TxReady);
	USART_SendData(USART1, c);
	TxReady = 0;
}
int USART1_Dequeue(char* c)
{
	int ret;
	ret = 0;
	*c = 0;
	NVIC_DisableIRQ(USART1_IRQn);
	if (RX_BUFFER_HEAD != RX_BUFFER_TAIL)
	{
	*c = RX_BUFFER[RX_BUFFER_TAIL];
	RX_BUFFER_TAIL++;
	if (RX_BUFFER_TAIL == BUFSIZE) RX_BUFFER_TAIL = 0;
	ret = 1;
	}
	NVIC_EnableIRQ(USART1_IRQn);
	return ret;
}




//Init Usart6
void USART6_Init(void)
{
	GPIO_InitTypeDef 							GPIO_InitStruct;
	USART_InitTypeDef 						USART_InitStruct;
	NVIC_InitTypeDef 							NVIC_InitStructure;
	
	// enable peripheral clocks (note: different bus interfaces for each peripheral!)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	// map port C pins for alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 	// Pins 6 (TX) and 7 (RX) will be used for USART1
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 						// GPIO pins defined as alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;			  // I/O pins speed (signal rise time)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 					// push-pull output
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 						// activates pullup resistors
	GPIO_Init(GPIOC, &GPIO_InitStruct); 									// set chosen pins
	
	// set alternate function to USART1 (from multiple possible alternate function choices)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); // pins will automatically be assignedto TX/RX - refer to datasheet to see AF mappings
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	// use USART_InitStruct to config USART1 peripheral
	USART_InitStruct.USART_BaudRate = BAUDRATE; 							// set baudrate from define
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	// 8 data bits
	USART_InitStruct.USART_StopBits = USART_StopBits_1; 			// 1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; 					// no parity check
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // no HW control flow
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enable both character transmit and receive
	USART_Init(USART6, &USART_InitStruct); 												// set USART1 peripheral

	// set interrupt triggers for USART1 ISR (but do not enable USART1 interrupts yet)
	USART_ITConfig(USART6, USART_IT_TXE, DISABLE);// should be disbled
	USART_ITConfig(USART6, USART_IT_TC, ENABLE); // transmission completed event (for reseting TxReady flag)
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); // character received (to trigger buffering of new character)

	TxReady = 1; 														// USART1 is ready to transmit
	RX_BUFFER_HEAD = 0; RX_BUFFER_TAIL = 0; // clear rx buffer

	// prepare NVIC to receive USART1 IRQs
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn; 					// configure USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// max. priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 					// max. priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// enable USART1 interrupt in NVIC
	NVIC_Init(&NVIC_InitStructure);															// set NVIC for USART1 IRQ
	
	// enables USART1 interrupt generation
	USART_Cmd(USART6, ENABLE);
}

void USART6_IRQHandler(void)
{
	static char rx_char;
	static char rx_head;
	// RX event
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
	USART_ClearITPendingBit(USART6, USART_IT_RXNE);
	rx_char = USART_ReceiveData(USART6);
	// check for buffer overrun:
	rx_head = RX_BUFFER_HEAD + 1;
	if (rx_head == BUFSIZE) rx_head = 0;
	if (rx_head != RX_BUFFER_TAIL)
	{
		// adding new char will not cause buffer overrun:
		RX_BUFFER[RX_BUFFER_HEAD] = rx_char;
		RX_BUFFER_HEAD = rx_head; // update head
	}
}

// TX event
	if (USART_GetITStatus(USART6, USART_IT_TC) == SET)
	{
	USART_ClearITPendingBit(USART6, USART_IT_TC);
	TxReady = 1;
	}
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


void USART6_SendChar(char c)
{
	while(!TxReady);
	USART_SendData(USART6, c);
	TxReady = 0;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int USART6_Dequeue(char* c)
{
	int ret;
	ret = 0;
	*c = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (RX_BUFFER_HEAD != RX_BUFFER_TAIL)
	{
		*c = RX_BUFFER[RX_BUFFER_TAIL];
		RX_BUFFER_TAIL++;
		if (RX_BUFFER_TAIL == BUFSIZE) RX_BUFFER_TAIL = 0;
		ret = 1;
	}
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

void readRequest(void)
{
	char request[2];
	char c;
	char x, array[4];								//heating or cooling
	int cnt=0, i; 
	char * ptr;
		GPIO_InitTypeDef GPIO_InitStructure;
	
					RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
				//pin PB7 and PB8 - PWM1 - configure as output
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
			GPIO_Init(GPIOD, &GPIO_InitStructure);
			
			
			
	
	memset(request, 0, sizeof(request));
	
	while(cnt<2)
		{
			if (USART6_Dequeue(&c))
			{
				request[cnt++]=c;
				USART6_SendChar(c);
			}
		}
		
	x=request[0];
//	temp = (request[3]-'0') + (request[2]- '0')* 10;
	temp=strtol(request, &ptr, 10);
	if(temp == 40){
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	}
	memcpy(array, &temp, sizeof(temp));

//		for(i=3; i>=0; i--)
//		{
//			USART6_SendChar(array[i]);
//		}		
}
		
			
//	if (x=='+')
//		{
//			Start_PWM1(duty_cycle);	
//		}
//		
//	if (x=='-')
//		{
//			Start_PWM2(duty_cycle);
//		}
//		
//	if (x=='0')
//		{
//			All_OFF();
//		}

//}