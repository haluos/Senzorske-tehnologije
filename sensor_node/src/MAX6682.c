#include <MAX6682.h>

void SPI1_Init (void){
	GPIO_InitTypeDef GPIO_SPI;
	SPI_InitTypeDef SPIInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_SPI.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_SPI.GPIO_Mode = GPIO_Mode_AF;
	GPIO_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_SPI.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_SPI);

	GPIO_SPI.GPIO_Pin =	GPIO_Pin_4;
	GPIO_SPI.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_SPI.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_SPI.GPIO_OType = GPIO_OType_PP;
	GPIO_SPI.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_SPI);

//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);

	SPIInit.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPIInit.SPI_Direction=SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx;
	SPIInit.SPI_DataSize = SPI_DataSize_16b ;
	SPIInit.SPI_CPOL = SPI_CPOL_Low;
	SPIInit.SPI_CPHA = SPI_CPHA_2Edge;
	SPIInit.SPI_NSS = SPI_NSS_Soft;
	SPIInit.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPIInit.SPI_FirstBit = SPI_FirstBit_MSB;
	SPIInit.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPIInit);

	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx);
	SPI_Cmd(SPI1, ENABLE);
	//	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

uint16_t max6682temp(void){
	uint16_t data = 0;
	float temp;

	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	SPI_I2S_SendData(SPI1, 0);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY));

	GPIO_SetBits(GPIOA, GPIO_Pin_4);

	if (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)){
		data = SPI_I2S_ReceiveData(SPI1);
		// temp = (data >> 5)*0.125;
	}
	return data;
//		timer2_wait_millisec(1000);
//		USART1_SendChar('X');
//		sprintf((int)polje, "%2.4f\n", temp);
//							for (i=0; i<strlen(polje); i++)
//							USART1_SendChar(polje[i]);
//							USART1_SendChar(0x0D);
//		timer2_wait_millisec(1000);
}
