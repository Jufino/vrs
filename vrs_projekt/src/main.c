#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <tft/ili9163.h>
#include <onewire/onewire.h>
#include <delay/delay.h>
#include <regulator/regulator.h>
#include "stm32l1xx.h"

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

int main(void) {

	lcdInitialise(LCD_ORIENTATION1);
	lcdClearDisplay(decodeRgbValue(0, 0, 0));
	OneWireInit();
	regulatorInit();

	float pozadovanaHodnota = 56; // tu nahradit hociakym druhom nastavovania ziadanej hodnoty
	setPozadovanaHodnota(pozadovanaHodnota);
	showPozadovanaHodnota(pozadovanaHodnota);
	showAktualnaHodnota(0);
	showAkcnyZasah(0);
	while (1) {
		float actualTemperature = getTemperature();
		showAktualnaHodnota(actualTemperature);
		showAkcnyZasah(setAktualnaHodnota(actualTemperature));
		DelayMs(200);
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
