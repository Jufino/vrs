#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>
#include "stm32l1xx.h"

void DelayUs(__IO uint32_t nTime);
void DelayMs(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif
