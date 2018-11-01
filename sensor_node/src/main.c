#include <main.h>

void _init(void){}

int main(void)
{
	int time=0;
	int index, flag = 0, bytes, counter = 0;
	float temp=0, t=0;
	char c, *ptr, request[7];
	uint8_t Opcode, pack[PACKSIZE];

	timer2_init();

	gpio_init();

	SysTick_Init();

	usart1_init();
	USART6_Init();
	Init_I2C_GPIO();

	ADC1_Init ();
	ADC2_Init();
	ADC3_Init ();

	SPI1_Init();
	SPI2_Init();
	buffer_initialization();
//	tmp101_init (0xE1);
//	Init_PWM1();
//	Init_PWM2();
//	t=310;
	while(1){
		if(check_flag()){
			t=0;
//			readRequest(request);
//			index = strtol(c, &ptr, 10);
//			for(int i=0; i<sizeof(request); i++){
//				USART6_SendChar(request[i]);
//				if(request[i] == '\0') break;
//				index = request[i] - '0';
//				send_duty(index);
				while(1){
//					get_temp(t, &temp);
//					tmp101_temp (&temp, &t);
//
//					Opcode=getOpcode(0x03, TIME, LSB_FIRST, CRC_YES, END_YES);
//					bytes = returnPacked(time, t, Opcode, counter++, pack);
//					fillBuffer(pack, bytes);
					if(timer_flag()){
//						t+=1;
						get_temp(t, &temp);
						// if(t == 300){
						// 	t=0;
						// }
						// else{
							t+=1;
						// }
//					if(!(timer2_get_millisec() % 1000)){
//						while(USART6_Dequeue(&c)){
//							USART6_SendChar(c);
//						}
						empty_buffer();
//						USART6_SendChar(0x0D); USART6_SendChar(0x0A);
						buffer_initialization();
//						break;
					}
					if(USART6_Dequeue(&c)){
						break;
					}
//					if(USART6_Dequeue(&c)){
//						if(c == '0'){
//							flag = 1;
//						}
//					}
//					USART6_SendChar('t');
//					if((temp>=(index+3)*10) || flag){
//						All_OFF();
//						empty_buffer();
//						flag = 0;
//						break;
//					}
//					t+=1;
				}
//			}
		}

//		readRequest();
//		if(requested_temp){
//			timer2_wait_millisec(50);
//			time += 50;
//	//		send_referent();
//			lm35dz_temp(&temp, &t);
//			if(temp==requested_temp){
//				All_OFF();
//			}
//			if((time % 1000) == 0){
//				get_temp(time);
//			}
//			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_PinSource1)){
//				empty_buffer();
//				time=0;
//			}
//		}
	}
}
