/*
 * kniznica.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Juraj
 */

#ifndef KNIZNICA_H_
#define KNIZNICA_H_
/*
 * kniznica.c
 *
 *  Created on: 18. 10. 2016
 *      Author: Juraj
 */

#include <stddef.h>
#include "stm32l1xx.h"

void initLed(void);
void adc_init(void);
int getADCValue(void);
void setLedToggle(void);
void delay(long time);

void initUART(void);
void sendCharUART(char znak);
void sendStringUART(char znak[],int length);

void sendCharByBufferUART(char znak);
void sendStringByBufferUART(char znak[], int lenght);
void callbackTXUART();

#endif /* KNIZNICA_H_ */
