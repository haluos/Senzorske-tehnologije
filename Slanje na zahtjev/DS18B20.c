#include <DS18B20.h>

void gpio_init() {
	GPIO_InitTypeDef GPIO;


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO.GPIO_Pin = GPIO_Pin_1;       		// select the pin to modify
  GPIO.GPIO_Mode = GPIO_Mode_OUT;       // set the mode to output
  GPIO.GPIO_Speed = GPIO_Speed_100MHz;  // set the I/O speed to 100 MHz
  GPIO.GPIO_OType = GPIO_OType_PP;      // set the output type to open-drain
  GPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;    // set the pull-up to none
  GPIO_Init(ONEWIRE_PORT, &GPIO);       // do the init
}



uint8_t  reset(void)
{
	ONEWIRE_CONFIG_OUTPUT;
	GPIO_SetBits(GPIOC, GPIO_Pin_1);
	delay_nus(500);

//	GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
	delay_nus(500);

	ONEWIRE_CONFIG_INPUT;
	delay_nus(60);

  if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)==0)
		{
			delay_nus(480);
			return 0;
		}
		else
		{
			delay_nus(480);
			return 1;
		}
}

uint8_t read(void) {
	uint8_t i, rez=0;
	ONEWIRE_CONFIG_INPUT;

	for (i=0; i<8; i++){
			ONEWIRE_CONFIG_OUTPUT;
			GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
			delay_nus(2);

			ONEWIRE_CONFIG_INPUT;
			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)!=Bit_RESET) {
			rez |= 1<<i;
			}
			delay_nus(60);
	}
	return rez;
}

void write(uint8_t val)
{
	uint8_t i;
	ONEWIRE_CONFIG_INPUT;

	for (i=0;i<8;i++)
	{
		if ((val & (1<<i))!=0)
		{
			ONEWIRE_CONFIG_OUTPUT;
			GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
			delay_nus(1);

			ONEWIRE_CONFIG_INPUT;
			delay_nus(60);
		}
		else {
			ONEWIRE_CONFIG_OUTPUT;
			GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
			delay_nus(60);
			ONEWIRE_CONFIG_INPUT;
		}
	}
}

uint16_t DS_temp(void){
	float temperatura=0;
	uint16_t temp=0;
	if (!reset())
	{
		uint8_t tempL, tempH, pom;
		float var;
		int i, n,j;

		write(0xCC);
		write(0x44);
		delay_nus(750);

		reset();
		write(0xCC);
		write(0xBE);

		tempL=read();
		tempH=read();

		temperatura=0;
		tempH=tempH & 0x07;
		temp=(tempH<<4) | (tempL>>4);
		// j=6;
		// for (i=0; i<7; i++){
		// 	pom=(tempH>>(6-i)) & 0x01;
		// 	var=1;
		// 	for(n=0; n<j; ++n){
		// 		var*=2;
		// 	}
		// 	temperatura+=(float)pom * var;
		// 	j--;
		// }
		// if(tempL & 0x1) temperatura = (float)(temperatura + (float)0.0625);
		// if(tempL & 0x2) temperatura = (float)(temperatura + (float)0.1250);
		// if(tempL & 0x4) temperatura = (float)(temperatura + (float)0.2500);
		// if(tempL & 0x8) temperatura = (float)(temperatura + (float)0.5000);

//				USART1_SendChar('D');
//				sprintf((int *)polje, "%2.4f\n", temperatura);
//							for (i=0; i<strlen(polje); i++)
//							USART1_SendChar(polje[i]);
//							USART1_SendChar(0x0D);

//							delay_nus(1000000);
	}
	return temp;
}
