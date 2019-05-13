#include "main.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "PWM.h"

#define PRINTF(a, b) {char string[100]; sprintf(string, a, b); int i; for(i=0; i<strlen(b); ++i) USART1_SendChar(string[i]);};

void Delay(__IO uint32_t nCount);

int main(void)
{
	float temp=0;
	uint32_t t=0;

	timer2_init();
	// gpio_init();
	SysTick_Init();
	Init_I2C_GPIO();
	ADC1_Init ();
	ADC2_Init();
	ADC3_Init ();
	SPI1_Init();
	SPI2_Init();
	buffer_initialization();
	USART6_Init();
	timer2_init();
	Init_PWM1();
	Init_PWM2();
	reset_pins();

	while(1)
	{
		readRequest();
		if(continue_measure()){
			if(check_ds()){
				DS_temp(&temp);
				reset_ds();
			}
			if(check_tmp()){
				tmp101_init (0xE1);
				tmp_temp(&temp);
				reset_tmp();
			}
			if(timer_flag()){
				get_temp(t, &temp);
				t+=1;
				empty_buffer();
				buffer_initialization();
			}
		}
		if(!continue_measure()){
			t=0;
		}
		// timer2_wait_millisec(1000);
	}
}
