#include "main.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "PWM.h"

#define PRINTF(a, b) {char string[100]; sprintf(string, a, b); int i; for(i=0; i<strlen(b); ++i) USART1_SendChar(string[i]);};
	
void Delay(__IO uint32_t nCount);
 
int main(void)
{
	USART6_Init();
	timer2_init();
	Init_PWM1();
	Init_PWM2();
	reset_pins();
	ADC1_Init ();
	
	char c;

	while(1)
	{

		readRequest();
	}
}
