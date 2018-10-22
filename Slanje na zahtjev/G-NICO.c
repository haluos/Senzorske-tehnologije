#include "G-NICO.h"

void ADC2_Init (void){
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef adc_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	ADC_StructInit(&adc_init);
	adc_init.ADC_Resolution = ADC_Resolution_10b;
	ADC_Init(ADC2, &adc_init);
	ADC_Cmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_144Cycles);
}

float NICO_temp(void) {
	uint16_t data;
	float temp, brojnik, nazivnik, otpor, a, b, c, d, e, ad, Vcc;
	int i;
	char array[10];
	Vcc=3.3;
	ADC_SoftwareStartConv(ADC2);
	data=ADC_GetConversionValue(ADC2);
	temp=((data)*Vcc)/1023.0;
	brojnik=(float)(56000*temp);
	nazivnik=(float)(43.0*Vcc - temp);
	a=-412.6;
	b=140.41;
	c=0.00764;
	d=-6.25*pow(10,-17);
	e=(-1.25)*pow(10, -24);
	otpor=brojnik/nazivnik;
	
	ad=(float)(a+b*sqrt(1+c*otpor)+d*pow(otpor,5)+e*pow(otpor,7));
	return ad;
//	sprintf((int *)(array), "%3.4f\n", (float)ad);
//	USART1_SendChar('G');
//	for(i=0; i<sizeof(array)-3; ++i){
//			USART1_SendChar(array[i]);
//		}
//	USART1_SendChar(0x0A);
//	USART1_SendChar(0x0D);
//	timer2_wait_millisec (1000);
}
