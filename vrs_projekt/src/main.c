#include <stddef.h>
#include "stm32l1xx.h"
#include "kniznica.h"
#include "ili9163.h"

volatile int hodnotaADC = 0;
volatile int toggleBitUart = 0;

void ADC1_IRQHandler(void) {
	if (ADC1->SR & ADC_SR_EOC) {
		hodnotaADC = ADC1->DR;
	}
}

void USART2_IRQHandler(void) {
	uint8_t pom = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		pom = USART_ReceiveData(USART2);
		if (pom == 'm') {
			if (toggleBitUart == 1)
				toggleBitUart = 0;
			else
				toggleBitUart = 1;
		}
	}
	else if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_TC);
		callbackTXUART();
	}
}

int main(void)
{
  int i = 0;

  lcdInitialise(LCD_ORIENTATION0);
  lcdClearDisplay(decodeRgbValue(0, 0, 0));
  lcdPutS("Hello", 20, 20, decodeRgbValue(0, 0, 0), decodeRgbValue(31, 31, 31));

    while (1)
  {

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