#include "usart.h"

uint8_t request_buffer[BUFSIZE];
int head_request, tail_request;
uint8_t buffer[BUFSIZE];
int head, tail;
int txReady, request_ready;


uint8_t RX_BUFFER[BUFSIZE];
int RX_BUFFER_HEAD, RX_BUFFER_TAIL;
uint8_t TxReady;

//---------------------------------------------------------------------------------------------------------------------------
void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	USART_InitStruct.USART_BaudRate = BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	head=0;
	tail=0;
	head_request=0;
	tail_request=0;

	txReady=1;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

//--------------------------------------------------------------------------------------------------------------------------
void USART1_IRQHandler (void)
{
	uint8_t data;
	int new_head;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data=USART_ReceiveData(USART1);
		new_head=head_request+1;
		if(new_head==BUFSIZE) new_head=0;

		if(new_head!=tail_request)
		{
			request_buffer[head_request]=data;
			head_request=new_head;
		}
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		txReady=1;
	}
}

//----------------------------------------------------------------------------------------------------------------------------
//Function sendData waits till current transmission to send next data

void sendData (uint8_t data)
{
	while(!TxReady);
	USART_SendData(USART6, data);
	TxReady=0;
}

void sendPack (uint8_t *pack, int size)
{
	int i;
	for (i=0; i<size; ++i)
	{
		sendData(pack[i]);
		//if(i==(size+1)) gpio_led_state (LED6_BLUE_ID, 1);
	}
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

//---------------------------------------------------------------------------------------------------------------------------
int getRequest (uint8_t* data)
{
	int ret=1;
	*data = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (head_request != tail_request)
	{
		*data = request_buffer[tail_request];
		tail_request++;
		if (tail_request == BUFSIZE) tail_request = 0;
	}
	else ret=0;
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}

//--------------------------------------------------------------------------------------------------------------------------
int push_Buffer (uint8_t data)
	{
	int new_head, ret=1;
	new_head=head+1;
//		new_head=RX_BUFFER_HEAD+1;
	
	if(new_head==BUFSIZE) new_head=0;

	if(new_head!=tail)
	{
		buffer[head]=data;
//		RX_BUFFER[RX_BUFFER_HEAD] = data;
		head=new_head;
//		RX_BUFFER_HEAD = new_head;
	}
	else
		ret=0;
	return ret;
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

//--------------------------------------------------------------------------------------------------------------------------
void DMA_SendData (int counter){
	DMA_InitTypeDef DMA2_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	while(DMA_GetCmdStatus(DMA2_Stream7) == ENABLE);
	DMA2_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA2_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA2_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA2_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA2_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA2_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA2_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA2_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA2_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA2_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA2_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA2_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA2_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA2_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&buffer;
	DMA2_InitStruct.DMA_BufferSize = (uint16_t)counter;
	
	DMA_DeInit(DMA2_Stream7);
	DMA_Init(DMA2_Stream7, &DMA2_InitStruct);
		
	DMA_Cmd(DMA2_Stream7, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------------
void buffer_initialization (void){
	RX_BUFFER_HEAD=0;
	RX_BUFFER_TAIL=0;
}

//--------------------------------------------------------------------------------------------------------------------------
void empty_buffer (void){
	uint8_t data;
	while(pop_Buffer(&data)){
//		sendData(data);
		USART6_SendChar(data);
	}
}

//--------------------------------------------------------------------------------------------------------------------------
void readRequest(char *request)
{
//	char request[7];
	char c;
	int cnt=0, i; 
	char * ptr, read[7];
	
	memset(request, 0, sizeof(request));
		
//	if(request_ready){
		while(1){
			if (USART6_Dequeue(&c)){
				if(c == '\n'){
					break;
				}
				read[cnt++] = c;
			}
		}
		sprintf(request, "%s", read);
//		for(i=0;i<sizeof(request);i++){
//			USART6_SendChar(request[i]);
//		}
//		temp=strtol(request, &ptr, 10);	
		request_ready = 0;
//	}
}

//--------------------------------------------------------------------------------------------------------------------------
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
	head = 0; tail = 0;

	// prepare NVIC to receive USART1 IRQs
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn; 					// configure USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// max. priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 					// max. priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						// enable USART1 interrupt in NVIC
	NVIC_Init(&NVIC_InitStructure);															// set NVIC for USART1 IRQ
	
	// enables USART1 interrupt generation
	USART_Cmd(USART6, ENABLE);
}

//--------------------------------------------------------------------------------------------------------------------------
void USART6_IRQHandler(void)
{
	static char data;
	static char new_head;
	// RX event
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
		request_ready = 1;
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		data=USART_ReceiveData(USART6);
		new_head=head_request+1;
		if(new_head==BUFSIZE) new_head=0;

		if(new_head!=tail_request)
		{
			request_buffer[head_request]=data;
			head_request=new_head;
		}
	}
	if (USART_GetITStatus(USART6, USART_IT_TC) == SET)
	{
	USART_ClearITPendingBit(USART6, USART_IT_TC);
	TxReady = 1;
	}
}

//--------------------------------------------------------------------------------------------------------------------------

int check_flag(void){
	char c;
	if(request_ready){
		if(USART6_Dequeue(&c)){
			request_ready = 0;
			return 1;
		}
	}
	return 0;
}


	
//--------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------
int USART6_Dequeue(char* c)
{
	int ret=1;
	*c = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (head_request != tail_request)
	{
		*c = request_buffer[tail_request];
		tail_request++;
		if (tail_request == BUFSIZE) tail_request = 0;
	}
	else ret=0;
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}

//---------------------------------------------------------------------------------------------------------------------
void USART6_SendChar(char c)
{
	while(!TxReady);
	USART_SendData(USART6, c);
	TxReady = 0;
}
		
