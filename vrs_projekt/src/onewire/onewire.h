#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <delay/delay.h>
#include "stm32l1xx.h"

#define ONEWIRE_PIN_NUM                 0
#define ONEWIRE_PIN_MASK                (1<<ONEWIRE_PIN_NUM)
#define ONEWIRE_PORT                    GPIOA
#define ONEWIRE_CLK                     RCC_AHBPeriph_GPIOA

#define ONEWIRE_INPUT_READ             ONEWIRE_PORT->IDR&ONEWIRE_PIN_MASK
#define ONEWIRE_OUTPUT_HIGH            ONEWIRE_PORT->BSRRL=ONEWIRE_PIN_MASK
#define ONEWIRE_OUTPUT_LOW             ONEWIRE_PORT->BSRRH=ONEWIRE_PIN_MASK
#define ONEWIRE_CONFIG_OUTPUT          ONEWIRE_PORT->MODER|=(GPIO_Mode_OUT<<(ONEWIRE_PIN_NUM*2))
#define ONEWIRE_CONFIG_INPUT           ONEWIRE_PORT->MODER&=~(GPIO_MODER_MODER0<<(ONEWIRE_PIN_NUM*2))

#include "onewire/onewire.h"

void OneWireInit(void);
void SendInitialization(void);
void SendByte(uint8_t val);
uint8_t ReadByte(void);
void ReportTemperature(void);


#endif
