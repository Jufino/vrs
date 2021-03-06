#include <stddef.h>
#include "stm32l1xx.h"
#include "kniznica.h"

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

int main(void) {
	char bufferPrevod[7];

	adc_init();
	initLed();
	initUART();
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	/* Infinite loop */
	while (1) {
		//setLedToggle();
		//delay(hodnotaADC * 100);
			if (toggleBitUart == 1){
				int prevedenaHodnota = (int)((3.3 / 4096) * hodnotaADC * 10);
				bufferPrevod[0] = ((char)prevedenaHodnota)/10;
				bufferPrevod[1] = '.';
				bufferPrevod[2] = (char) (prevedenaHodnota-bufferPrevod[0]*10) + '0';
				bufferPrevod[0] += '0';
				bufferPrevod[3] = 'V';
				bufferPrevod[4] = ' ';
				bufferPrevod[5] = '\r';
				bufferPrevod[6] = '\n';
			}
			else{
				int hodnotaUpravena = 0;
				bufferPrevod[0] = (char)(hodnotaADC/1000);
				hodnotaUpravena = (hodnotaADC-(int)bufferPrevod[0]*1000);
				bufferPrevod[1] = (char)(hodnotaUpravena/100);
				hodnotaUpravena = (hodnotaUpravena-(int)bufferPrevod[1]*100);
				bufferPrevod[2] = (char)(hodnotaUpravena/10);
				hodnotaUpravena = (hodnotaUpravena-(int)bufferPrevod[2]*10);
				bufferPrevod[3] = (char)(hodnotaUpravena)+'0';
				bufferPrevod[0] += '0';
				bufferPrevod[1] += '0';
				bufferPrevod[2] += '0';
				bufferPrevod[4] = ' ';
				bufferPrevod[5] = '\r';
				bufferPrevod[6] = '\n';
			}
			sendStringByBufferUART(bufferPrevod, 7);
			delay(100);
	}
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
