#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <tft/ili9163.h>
#include <delay/delay.h>
#include <regulator/regulator.h>
#include "stm32l1xx.h"

volatile static uint8_t bufferRead[5];
volatile static uint8_t index = 0;
volatile uint8_t dataReady = 0;
void USART3_IRQHandler(void) {
	uint8_t pom = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		pom = USART_ReceiveData(USART3);
		if(dataReady != 1){
		if(pom == '\n' || pom == '\r' || index > 5){
			bufferRead[index] = '\0';
			index = 0;
			dataReady = 1;
		}
		else{
			bufferRead[index++] = pom;
			dataReady = 0;
		}
		}
	}
}




void showPozadovanaHodnota(float temp){
	char buffer[20];
	const int decimalTemp = (int)temp;
	const int fragmetTemp = round((temp-decimalTemp)*10);
	const int riadok = 0;
	sprintf (buffer, "Pozadovana:%d.%d", decimalTemp,fragmetTemp);
	lcdRectangle(lcdTextX(0),lcdTextY(riadok),LCD_WIDTH,lcdTextY(riadok)+8,decodeRgbValue(0, 0, 0));
	lcdPutS(buffer, lcdTextX(0), lcdTextY(riadok), decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

void showAktualnaHodnota(float temp){
	char buffer[20];
	const int decimalTemp = (int)temp;
	const int fragmetTemp = round((temp-decimalTemp)*10);
	const int riadok = 1;
	sprintf (buffer, "Aktualna:%d.%d", decimalTemp,fragmetTemp);
	lcdRectangle(lcdTextX(0),lcdTextY(riadok),LCD_WIDTH,lcdTextY(riadok)+8,decodeRgbValue(0, 0, 0));
	lcdPutS(buffer, lcdTextX(0), lcdTextY(riadok), decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

void showAkcnyZasah(int status){
	char buffer[20];
	const int riadok = 2;
	sprintf (buffer, "Zasah:%d", status);
	lcdRectangle(lcdTextX(0),lcdTextY(riadok),LCD_WIDTH,lcdTextY(riadok)+8,decodeRgbValue(0, 0, 0));
	lcdPutS(buffer, lcdTextX(0), lcdTextY(riadok), decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
}

float getTemp(void){
	while(dataReady != 1);
	float temp = atof(bufferRead);
	dataReady= 0;
	return temp;
}

int main(void) {

	lcdInitialise(LCD_ORIENTATION1);
	lcdClearDisplay(decodeRgbValue(0, 0, 0));
	regulatorInit();
	initUART();

	float pozadovanaHodnota = 33; // tu nahradit hociakym druhom nastavovania ziadanej hodnoty
	float aktualnaHodnota = 0;
	setPozadovanaHodnota(pozadovanaHodnota);
	showPozadovanaHodnota(pozadovanaHodnota);
	showAktualnaHodnota(aktualnaHodnota);
	showAkcnyZasah(0);
	while (1) {
		aktualnaHodnota = getTemp();
		showAktualnaHodnota(aktualnaHodnota);
		showAkcnyZasah(setAktualnaHodnota(aktualnaHodnota));
		DelayMs(100);
	}
	return 0;
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func,
		const char *failedexpr) {
	while (1) {
	}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr) {
	__assert_func(file, line, NULL, failedexpr);
}
