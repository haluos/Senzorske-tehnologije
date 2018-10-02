#include <MAX6675.h>

void SPI2_Init (void){
	GPIO_InitTypeDef GPIO_SPI;
	SPI_InitTypeDef SPIInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_SPI.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; 
	GPIO_SPI.GPIO_Mode = GPIO_Mode_AF;			
	GPIO_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_SPI.GPIO_PuPd = GPIO_PuPd_UP;			
	
	GPIO_Init(GPIOB, &GPIO_SPI);
	
	GPIO_SPI.GPIO_Pin =	GPIO_Pin_9;
	GPIO_SPI.GPIO_Mode = GPIO_Mode_OUT;			
	GPIO_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_SPI.GPIO_PuPd = GPIO_PuPd_UP;	
	
	GPIO_Init(GPIOB, &GPIO_SPI);
	
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	
	SPIInit.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
	//SPIInit.SPI_Direction=SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx;
	SPIInit.SPI_DataSize = SPI_DataSize_16b ;
	SPIInit.SPI_CPOL = SPI_CPOL_Low;
	SPIInit.SPI_CPHA = SPI_CPHA_2Edge;
	SPIInit.SPI_NSS = SPI_NSS_Soft;
	SPIInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPIInit.SPI_FirstBit = SPI_FirstBit_MSB;
	SPIInit.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI2, &SPIInit);
	
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx);
	SPI_Cmd(SPI2, ENABLE);
	//	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

float max6675temp(void){
	uint16_t data = 0;
	float temp;

	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	SPI_I2S_SendData(SPI2, 0);
	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY));
		
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
	if (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)){
		data = SPI_I2S_ReceiveData(SPI2);
		temp = (data >> 3) * 0.25;
	}
	return temp;
//		timer2_wait_millisec(1000);
//		USART1_SendChar('M');
//		sprintf((int)polje, "%2.4f\n", temp);
//							for (i=0; i<strlen(polje); i++)
//							USART1_SendChar(polje[i]);
//							USART1_SendChar(0x0D);
//		timer2_wait_millisec(1000);
}

