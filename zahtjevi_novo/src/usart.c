/* usart.c */
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "PWM.h"

#define PRINTF(a, b) {char string[100]; sprintf(string, a, b); int i; for(i=0; i<strlen(b); ++i) USART1_SendChar(string[i]);};

uint8_t buffer[BUFSIZE];
int head, tail;
int txReady, request_ready;

// RX FIFO buffer
char RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;
unsigned char request_buffer[12]; int request_head, request_tail;
// TX state flag
uint8_t TxReady;
int cnt=0, request_ready, requested_temp = 0, start_meas = 0;
//	druga vrijednost se uzima za temperaturu visu ili nizu od trazene za 20 stupnjeva
int look_up[6][2] = { {6500, 6500}, {10000, 10000}, {15000, 12500}, {17000, 15000}, {17000, 20000}, {20000, 22000} };

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
	request_head = 0; request_tail = 0;

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
	// static char rx_head;
	int req_head;
	// RX event
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		rx_char = USART_ReceiveData(USART6);
		cnt++;
		if(cnt == 2){
			request_ready = 1;
		}
		req_head = request_head +1;
		if(req_head == 12) req_head = 0;
		if(req_head != request_tail){
			request_buffer[request_head] = rx_char;
			if(request_head == 5 || request_head == 11){
				// start_meas = 1;
				requested_temp = 1;
			}
			request_head = req_head;
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

void USART6_SendChar(char c)
{
	while(!TxReady);
	USART_SendData(USART6, c);
	TxReady = 0;
}

//----------------------------------------------------------------------------------------------------------------------------
//Function fillBuffer fills FIFO buffer with data
//Arguments:	pack -	packed data to be put in buffer
//						size -	number of bytes in pack
//Return value:	returns 0 if FIFO buffer is full

int fillBuffer (uint8_t *pack, int size)
{
	int i, ret=1, new_head;
	for(i=0; i<(size); ++i)
	{
//		new_head=head+1;
		new_head = RX_BUFFER_HEAD+1;

	if(new_head==BUFSIZE) new_head=0;

	if(new_head!=tail)
	{
//		buffer[head]=pack[i];
		RX_BUFFER[RX_BUFFER_HEAD]=pack[i];
//		head=new_head;
		RX_BUFFER_HEAD = new_head;
	}
	else
		ret=0;
	}
	return ret;
}

//-------------------------------------------------------------------------------------------------------------------------
void buffer_initialization (void){
	RX_BUFFER_HEAD=0;
	RX_BUFFER_TAIL=0;
}

//--------------------------------------------------------------------------------------------------------------------------
int pop_Buffer (uint8_t *data){
	int ret=1;
	*data = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (RX_BUFFER_HEAD != RX_BUFFER_TAIL)
	{
//		*data = buffer[tail];
		*data = RX_BUFFER[RX_BUFFER_TAIL];
//		tail++;
		RX_BUFFER_TAIL++;
//		if (tail == BUFSIZE) tail = 0;
		if (RX_BUFFER_TAIL == BUFSIZE) RX_BUFFER_TAIL = 0;
	}
	else ret=0;
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}


void empty_buffer (void){
	uint8_t data;
	while(pop_Buffer(&data)){
//		sendData(data);
		USART6_SendChar(data);
	}
}

//-------------------------------------------------------------------------------------------------

int USART6_Dequeue(char* c)
{
	int ret;
	ret = 0;
	*c = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (request_head != request_tail)
	{
		*c = request_buffer[request_tail];
		request_tail++;
		if (request_tail == 12) request_tail = 0;
		ret = 1;
	}
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}

//--------------------------------------------------------------------------------------------------

int readRequest(void)
{
	char request[6];
	char c;
	char x;								//heating or cooling
	int count=0;
	int duty_cycle=0;			//duty cycle

	memset(request, 0, 6);

	// if(!requested_temp){
	// 	return 0;
	// }
	if(requested_temp){
		requested_temp = 0;
		while(count<6)
			{
				if (USART6_Dequeue(&c))
				{
					request[count]=c;
					count++;
				}
			}

		x=request[0];
		duty_cycle = (request[5]-'0') + (request[4]- '0')*10 + (request[3]- '0')*100  + (request[2]- '0')*1000 + (request[1]- '0')*10000;

		if (x=='+')
			{
				start_meas = 1;
				// GPIO_SetBits(GPIOB, GPIO_Pin_3);
				Start_PWM1(duty_cycle);
				// GPIO_ResetBits(GPIOB, GPIO_Pin_3);
				return 1;
			}

		if (x=='-')
			{
				start_meas = 1;
				// GPIO_SetBits(GPIOB, GPIO_Pin_3);
				Start_PWM2(duty_cycle);
				// GPIO_ResetBits(GPIOB, GPIO_Pin_3);
				return 1;
			}

		if (x=='0')
			{
				start_meas = 0;
				// GPIO_SetBits(GPIOB, GPIO_Pin_3);
				All_OFF();
				// GPIO_ResetBits(GPIOB, GPIO_Pin_3);
				return -1;
			}
		}
		return 0;
}

// ---------------------------------------------------------------------------------------------------
int continue_measure (void){
	return start_meas;
}
