#include <regulator/regulator.h>
int dynamicHysterese = 0;
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

void setAkcnyZasah(char status){
	if(status == 0){
	if(!REGULATOR_INVERSE)
		REGULATOR_OUTPUT_LOW;
	else
		REGULATOR_OUTPUT_HIGH;
	}
	else{
		if(!REGULATOR_INVERSE)
			REGULATOR_OUTPUT_HIGH;
		else
			REGULATOR_OUTPUT_LOW;
	}
}

void vygenerujAkcnyZasah(float pozadovana,float aktualna){
	if(aktualna >= (pozadovana+REGULATOR_HYSTERESIS_UP)){
		setAkcnyZasah(0);
	}
	else if(aktualna <= (pozadovana-REGULATOR_HYSTERESIS_DOWN)){
		setAkcnyZasah(1);
	}
}

char getAkcnyZasah(){
	if(REGULATOR_INVERSE)
		return !(REGULATOR_PORT->ODR&REGULATOR_PIN_NUM) && REGULATOR_PIN_NUM;
	else
		return (REGULATOR_PORT->ODR&REGULATOR_PIN_NUM) && REGULATOR_PIN_NUM;
}




