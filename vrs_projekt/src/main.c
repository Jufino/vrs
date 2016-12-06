#include <stdio.h>
#include <math.h>
#include <onewire/onewire.h>
#include <regulator/regulator.h>
#include <tft/ili9163.h>
#include "stm32l1xx.h"

#include  <stdio.h>

volatile float valuePozadovana = 0;
volatile float valueAktualna = 0;
volatile char isShowtextPozadovana = 0;
volatile char isShowtextAktualna = 0;
volatile char isShowtextAkcnyZasah = 0;
volatile float lastShowedValuePozadovana = -100;
volatile float lastShowedValueAktualna = -100;
volatile float lastShowedValueZasah = -100;

void showPozadovanaHodnota(float temp) {
	if (temp != lastShowedValuePozadovana) {
		char buffer[20];
		const int decimalTemp = (int) temp;
		const int fragmetTemp = round((temp - decimalTemp) * 10);
		const int riadok = 0;
		sprintf(buffer, "%d.%d", decimalTemp, fragmetTemp);

		if (!isShowtextPozadovana) {
			lcdPutS("Pozadovana:", lcdTextX(0), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		lcdPutS("00.00", lcdTextX(12), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lcdPutS(buffer, lcdTextX(12), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValuePozadovana = temp;
	}

}

void showAktualnaHodnota(float temp) {
	if (temp != lastShowedValueAktualna) {
		char buffer[20];
		const int decimalTemp = (int) temp;
		const int fragmetTemp = round((temp - decimalTemp) * 10);
		const int riadok = 1;
		sprintf(buffer, "%d.%d", decimalTemp, fragmetTemp);

		if (!isShowtextPozadovana) {
			lcdPutS("Aktualna:", lcdTextX(0), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		lcdPutS("00.00", lcdTextX(12), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lcdPutS(buffer, lcdTextX(12), lcdTextY(riadok),
				decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValueAktualna = temp;
	}
}

void showAkcnyZasah(int status) {
	if (lastShowedValueZasah != status) {
		const int riadok = 2;
		if (!isShowtextPozadovana) {
			lcdPutS("Zasah:", lcdTextX(0), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		}
		if (status == 0)
			lcdPutS("0", lcdTextX(12), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		else
			lcdPutS("1", lcdTextX(12), lcdTextY(riadok),
					decodeRgbValue(31, 31, 31), decodeRgbValue(0, 0, 0));
		lastShowedValueZasah = status;
	}
}

void ADC1_IRQHandler(void) {
	if (ADC1->SR & ADC_SR_EOC) {
		float hodnotaADC = ADC1->DR;
		valuePozadovana = (hodnotaADC / 80) + 20;
		showPozadovanaHodnota(valuePozadovana);
	}
}

void adc_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	/* Configure ADCx Channel 1 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		;
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* ADCx regular channel1 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_16Cycles);
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) {
	}
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn; //zoznam prerušení nájdete v súbore stm32l1xx.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

int main(void) {
	//delayInit();
	OW_Init();
	lcdInitialise(LCD_ORIENTATION1);
	lcdClearDisplay(decodeRgbValue(0, 0, 0));
	regulatorInit();
	adc_init();
	while (1) {
		ADC_SoftwareStartConv(ADC1);
		OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ);
		for (long i = 0; i < 10000; i++)
			;
		uint8_t buf[2];
		OW_Send(OW_SEND_RESET, "\xcc\xbe\xff\xff", 4, buf, 2, 2);
		//-----------------
		float teplota = (float) ((buf[1] << 8) | buf[0]) / 16.0;
		valueAktualna=teplota;
		showAktualnaHodnota(valueAktualna);
		vygenerujAkcnyZasah(valuePozadovana,teplota);
		showAkcnyZasah(getAkcnyZasah());
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
