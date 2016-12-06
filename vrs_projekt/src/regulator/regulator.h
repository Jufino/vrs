#ifndef REGULATOR_H_
#define REGULATOR_H_

#include "stm32l1xx.h"

#define REGULATOR_HYSTERESIS_UP 			  0
#define REGULATOR_HYSTERESIS_DOWN 			  0.5
#define REGULATOR_INVERSE				  1

#define REGULATOR_PIN_NUM                 GPIO_Pin_1
#define REGULATOR_PORT                    GPIOC
#define REGULATOR_CLK                     RCC_AHBPeriph_GPIOC
#define REGULATOR_OUTPUT_HIGH            REGULATOR_PORT->BSRRL=REGULATOR_PIN_NUM
#define REGULATOR_OUTPUT_LOW             REGULATOR_PORT->BSRRH=REGULATOR_PIN_NUM

void regulatorInit(void);

void vygenerujAkcnyZasah(float pozadovana,float aktualna);

void setAkcnyZasah(char status);

char getAkcnyZasah();


#endif /* REGULATOR_H_ */
