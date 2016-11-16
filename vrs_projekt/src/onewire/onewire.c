#include "onewire/onewire.h"

/*
 *  OneWire_Init      hardware-specific configuration of 1-wire I/O
 */
void OneWireInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(ONEWIRE_CLK, ENABLE);        // route the clocks

	GPIO_InitStructure.GPIO_Pin = ONEWIRE_PIN_MASK;  // select the pin to modify
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      // set the mode to output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz; // set the I/O speed to 100 MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; // set the output type to open-drain
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  // set the pull-up to none
	GPIO_Init(ONEWIRE_PORT, &GPIO_InitStructure);                // do the init
}

void SendInitialization(void) {
	ONEWIRE_OUTPUT_HIGH;
	ONEWIRE_CONFIG_OUTPUT;
	DelayUs(500);

	ONEWIRE_OUTPUT_LOW;
	DelayUs(500);

	ONEWIRE_OUTPUT_HIGH;
	ONEWIRE_CONFIG_INPUT;
	DelayUs(50);
}

void SendByte(uint8_t val) {
	uint8_t n;

	for (n = 0; n < 8; n++) {
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		DelayUs(5);
		if (val & 1)
			ONEWIRE_OUTPUT_HIGH;
		DelayUs(95);
		ONEWIRE_OUTPUT_HIGH;
		DelayUs(5);
		val = val >> 1;
	}
}

uint8_t ReadByte(void) {
	uint8_t n;
	uint8_t val;

	val = 0;
	for (n = 0; n < 8; n++) {
		val = val >> 1;
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		DelayUs(15);
		ONEWIRE_OUTPUT_HIGH;
		ONEWIRE_CONFIG_INPUT;
		DelayUs(10);
		if (ONEWIRE_INPUT_READ)
			val = val | 0x80;
		DelayUs(35);
	}
	return val;
}
#define SKIP_ROM 0xCC
#define READ_SCRATCHPAD 0xBE
void ReportTemperature(void) {
	uint32_t val;
	uint32_t t;
	uint32_t frac;
	uint8_t n;
	uint8_t pad[10];
	SendInitialization();
	DelayUs(100);
	SendByte(SKIP_ROM);
	SendByte(READ_SCRATCHPAD);
	for (n = 0; n < 9; n++) {
		pad[n] = ReadByte();
	}
	val = (pad[1] * 256 + pad[0]);             // temp in 0.5 degs C
	t = val;
	val = val >> 1;                            // temp in degs C
	frac = 0;
	if ((val << 1) != t)
		frac = 5;            // if the roll lost a bit, allow for 0.5 deg C
	printf("\n\rTemperature is: %d.%d degrees C", val, frac);
}
