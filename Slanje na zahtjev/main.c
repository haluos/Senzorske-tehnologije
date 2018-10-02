#include <main.h>

int main(void)
{
	int time=0;
	int requested_temp = 0;
	float temp=0, t;
	timer2_init();
	gpio_init();
	SysTick_Init();
	usart1_init();
	USART6_Init();
	Init_I2C_GPIO();
	ADC1_Init ();
	SPI1_Init();
	SPI2_Init();
	Init_PWM1();
	Init_PWM2();
	
	while(1){
		requested_temp=readRequest();
		if(requested_temp){
			timer2_wait_millisec(50);
			time += 50;
	//		send_referent();
			lm35dz_temp(&temp, &t);
			if(temp==requested_temp){
				All_OFF();
			}
			if((time % 1000) == 0){
				get_temp(time);
			}
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_PinSource1)){
				empty_buffer();
				time=0;
			}
		}
	}
}
