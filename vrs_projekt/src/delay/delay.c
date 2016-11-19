#include "delay/delay.h"

RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay;
uint8_t canSetToDefault=0;

void DelayUs(__IO uint32_t nTime) {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	while (SysTick_Config(RCC_Clocks.HCLK_Frequency/1000000) != 0); //prestavime volanie handlera na kazdu 1 us
	TimingDelay = nTime/10;
	canSetToDefault = 1;
	while (TimingDelay != 0);
}

void DelayMs(__IO uint32_t nTime) {
	RCC_GetClocksFreq(&RCC_Clocks);
	while (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000) != 0); //prestavime volanie handlera na kazdu 1 ms
	TimingDelay = nTime/2;
	canSetToDefault = 1;
	while (TimingDelay != 0);
}

void TimingDelay_Decrement(void) {
	if (TimingDelay != 0x00)
		TimingDelay--;
	else if(canSetToDefault == 1 && TimingDelay==0)
		SysTick_Config(RCC_Clocks.HCLK_Frequency); //nastavime naspet male ticky aby sme nezatazovali procesor
}

void DelayUsNOP(long time) {
	for (long i = 0; i < time; i++){
		__ASM volatile ("NOP");
		__ASM volatile ("NOP");
		__ASM volatile ("NOP");
		__ASM volatile ("NOP");
		__ASM volatile ("NOP");
	}
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
	__ASM volatile ("NOP");
}
