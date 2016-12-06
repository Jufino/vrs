#include <regulator/regulator.h>

void regulatorInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(REGULATOR_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = REGULATOR_PIN_NUM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(REGULATOR_PORT, &GPIO_InitStructure);

	//defaultne vypnute
	if(!REGULATOR_INVERSE)
		REGULATOR_OUTPUT_LOW;
	else
		REGULATOR_OUTPUT_HIGH;
}

void vygenerujAkcnyZasah(float pozadovana,float aktualna){
	float odchylka = pozadovana-aktualna;

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
}

char getAkcnyZasah(){
	if(REGULATOR_INVERSE)
		return !(REGULATOR_PORT->ODR&REGULATOR_PIN_NUM) && REGULATOR_PIN_NUM;
	else
		return (REGULATOR_PORT->ODR&REGULATOR_PIN_NUM) && REGULATOR_PIN_NUM;
}




