#include "delay/delay.h"

void Delay_init(){
	RCC_GetClocksFreq(&RCC_Clocks);
	//SysTick_Config(1000000);
}

void DelayUs(__IO uint32_t nTime) {
	TimingDelay = nTime;

	while (TimingDelay != 0)
		;
}

void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00) {
		TimingDelay--;
	}
}
