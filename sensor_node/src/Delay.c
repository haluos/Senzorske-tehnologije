#include "Delay.h"

static __IO uint32_t sysTickCounter;

void SysTick_Handler(void) {
	TimeTick_Decrement();
}

void SysTick_Init(void) {
	/****************************************
	 *SystemFrequency/1000      1ms         *
	 *SystemFrequency/100000    10us        *
	 *SystemFrequency/1000000   1us         *
	 *****************************************/
// Enable the TIM2 global Interrupt

	while (SysTick_Config(SystemCoreClock / 1000000) != 0) {
	} // One SysTick interrupt now equals 1us

}

/**
 * This method needs to be called in the SysTick_Handler
 */
void TimeTick_Decrement(void) {
	if (sysTickCounter != 0x00) {
		sysTickCounter--;
	}
}

void delay_nus(uint32_t n) {
	sysTickCounter = n/3;
	while (sysTickCounter != 0) {
	}
}

void delay_1ms(void) {
	sysTickCounter = 1000;
	while (sysTickCounter != 0) {
	}
}

void delay_nms(uint32_t n) {
	while (n--) {
		delay_1ms();
	}
}
