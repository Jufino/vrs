#include "onewire/onewire.h"

void OneWireInit(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(ONEWIRE_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = ONEWIRE_PIN_MASK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ONEWIRE_PORT, &GPIO_InitStructure);
}

uint8_t SendReset(void) {
	uint8_t r;
	uint8_t retries = 125;

	//noInterrupts();
	ONEWIRE_CONFIG_INPUT;
	//interrupts();
	// wait until the wire is high... just in case
	do {
		if (--retries == 0) return 0;
		DelayUsNOP(2000);
	} while (!ONEWIRE_INPUT_READ);

	//noInterrupts();
	ONEWIRE_OUTPUT_LOW;
	ONEWIRE_CONFIG_OUTPUT;	// drive output low
	//interrupts();
	DelayUsNOP(480);
	//noInterrupts();
	ONEWIRE_CONFIG_INPUT;	// allow it to float
	DelayUsNOP(70);
	r = !ONEWIRE_INPUT_READ;
	//interrupts();
	DelayUsNOP(410);
	return r;
}

void SendBit(uint8_t v)
{
	if (v & 1) {
		//noInterrupts();
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		DelayUsNOP(10);
		ONEWIRE_OUTPUT_HIGH;
		//interrupts();
		DelayUsNOP(50);
	} else {
		//noInterrupts();
		ONEWIRE_OUTPUT_LOW;
		ONEWIRE_CONFIG_OUTPUT;
		DelayUsNOP(70);
		ONEWIRE_OUTPUT_HIGH;
		//interrupts();
		DelayUsNOP(5);
	}
}

void SendByte(uint8_t val,uint8_t power) {
	uint8_t n;
	for (n = 0; n < 8; n++) {
		SendBit(val & 1);
		val = val >> 1;
	}
    	ONEWIRE_CONFIG_OUTPUT;
    	ONEWIRE_OUTPUT_HIGH;
}

uint8_t ReadBit(void)
{
	uint8_t r;

	//noInterrupts();
	ONEWIRE_CONFIG_OUTPUT;
	ONEWIRE_OUTPUT_LOW;
	ONEWIRE_CONFIG_INPUT;
	DelayUsNOP(10);
	r = ONEWIRE_INPUT_READ;
	//interrupts();
	DelayUsNOP(60);
	return r;
}

uint8_t ReadByte(void) {
	uint8_t n;
	uint8_t val;

	val = 0;
	for (n = 0; n < 8; n++) {
		val = val >> 1;
		if (ReadBit())
			val = val | 0x80;
		DelayUsNOP(35);
	}
	return val;
}

void SendSelect(const uint8_t rom[8])
{
    uint8_t i;
    SendByte(0x55,0);
    for (i = 0; i < 8; i++) SendByte(rom[i],0);
}

#define CONVERT 0x44
#define SKIP_ROM 0xCC
#define WRITE_SCRATCHPAD 0x4E
#define READ_SCRATCHPAD 0xBE
#define COPY_SCRATCH     0x48
float getTemperature(void) {
	uint8_t n;
	uint8_t pad[10];
	const uint8_t address[] = {0x28, 0x5E, 0x02, 0xF5, 0x00, 0x00, 0x00, 0x50};
/*
	SendReset();
	SendSelect(address);
	SendByte(WRITE_SCRATCHPAD);
	SendByte(120); // high alarm temp
	SendByte(-55); // low alarm temp

	SendByte(0x7F); // nastav 12 bit resolution
    // save the newly written values to eeprom
	SendByte(COPY_SCRATCH);
	DelayUsNOP(20);*/
/*
	SendReset();
	SendSelect(address);
	SendByte(CONVERT);
	DelayMs(1000);
	SendReset();
	SendSelect(address);
	SendByte(READ_SCRATCHPAD);
*/
	SendReset();
	SendSelect(address);
	SendByte(CONVERT,1);
	DelayMs(3000);
	SendReset();
	SendSelect(address);
	SendByte(READ_SCRATCHPAD,0);

	for (n = 0; n < 9; n++) {
		pad[n] = ReadByte();
	}

	int16_t val = ((pad[1]&0x07) << 8) | pad[0];
	int8_t cfg = (pad[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) val = val & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) val = val & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) val = val & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time

	float temperature = (float)(val) /16;
	if((pad[1]&0xF8) == 0xF8) temperature = temperature*-1;
	return temperature;
}
