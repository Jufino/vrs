#include <regulator/regulator.h>

float pozadovana;

void regulatorInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(REGULATOR_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = REGULATOR_PIN_NUM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(REGULATOR_PORT, &GPIO_InitStructure);

	//defaultne vypnute
	if(!REGULATOR_INVERSE)
		REGULATOR_OUTPUT_LOW;
	else
		REGULATOR_OUTPUT_HIGH;
}

void setPozadovanaHodnota(float value){
	pozadovana = value;
}

int setAktualnaHodnota(float value){
	float odchylka = pozadovana-value;

	if(odchylka > REGULATOR_HYSTERESIS){
		if(!REGULATOR_INVERSE)
			REGULATOR_OUTPUT_HIGH;
		else
			REGULATOR_OUTPUT_LOW;
	}
	else if(odchylka < -REGULATOR_HYSTERESIS){
		if(!REGULATOR_INVERSE)
			REGULATOR_OUTPUT_LOW;
		else
			REGULATOR_OUTPUT_HIGH;
	}
	return (GPIOA->ODR&REGULATOR_PIN_MASK) && REGULATOR_PIN_MASK;
}
