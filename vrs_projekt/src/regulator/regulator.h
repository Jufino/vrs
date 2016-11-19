#ifndef REGULATOR_H_
#define REGULATOR_H_

#include "stm32l1xx.h"

#define REGULATOR_HYSTERESIS 			  4
#define REGULATOR_INVERSE				  0

#define REGULATOR_PIN_NUM                 1
#define REGULATOR_PIN_MASK                (1<<REGULATOR_PIN_NUM)
#define REGULATOR_PORT                    GPIOA
#define REGULATOR_CLK                     RCC_AHBPeriph_GPIOA
#define REGULATOR_OUTPUT_HIGH            REGULATOR_PORT->BSRRL=REGULATOR_PIN_MASK
#define REGULATOR_OUTPUT_LOW             REGULATOR_PORT->BSRRH=REGULATOR_PIN_MASK

void regulatorInit();
void setPozadovanaHodnota(float value);
int setAktualnaHodnota(float value);

#endif /* REGULATOR_H_ */
