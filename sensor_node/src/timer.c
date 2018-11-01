#include <timer.h>

uint32_t timer2_Ticks_Millisec;
int millisec;

void timer2_init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure; 
	uint16_t TimerPeriod; 
	RCC_ClocksTypeDef RCC_Clocks; 
	uint32_t APB1_CLK; 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 

	RCC_GetClocksFreq(&RCC_Clocks); 
	APB1_CLK = RCC_Clocks.PCLK1_Frequency; 

	TimerPeriod = (uint16_t)(((APB1_CLK * 2)/ 1000 ) - 1); 

	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	timer2_Ticks_Millisec = 0;
	millisec = 0;

	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler (void)
{
	if(TIM_GetITStatus (TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit (TIM2, TIM_IT_Update);
		timer2_Ticks_Millisec++;
		if(!(timer2_Ticks_Millisec % 1000)){
//			timer2_Ticks_Millisec = 0;
			millisec = 1;
//			empty_buffer();
		}
	}
}

int timer_flag (void){
	if(millisec){
		millisec = 0;
		return 1;
	}
	else return 0;
}

void timer2_wait_millisec (uint32_t ms)
{
	uint32_t t1, t2;
	t1 = timer2_get_millisec();
	while(1)
	{
		t2 = timer2_get_millisec();
		if((t2-t1) >= ms) break;
		if(t2<t1) break;
	}
}

uint32_t timer2_get_millisec()
{
	uint32_t value;
	NVIC_DisableIRQ (TIM2_IRQn);
	value = timer2_Ticks_Millisec;
	NVIC_EnableIRQ (TIM2_IRQn);
	return value;
}
