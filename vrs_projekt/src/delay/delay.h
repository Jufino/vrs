#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "stm32l1xx.h"

static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

void Delay_init();
void DelayUs(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif
