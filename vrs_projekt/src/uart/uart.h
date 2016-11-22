/*
 * uart.h
 *
 *  Created on: 22. 11. 2016
 *      Author: Juraj
 */

#ifndef UART_UART_H_
#define UART_UART_H_
#include "stm32l1xx.h"
void initUART(void);

void sendCharUART(char znak);

void sendStringUART(char znak[], int length);

void sendCharByBufferUART(char znak);

void sendStringByBufferUART(char znak[], int length);

void callbackTXUART();



#endif /* UART_UART_H_ */
