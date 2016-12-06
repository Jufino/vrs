#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "stm32l1xx.h"

#define OW_SEND_RESET		1
#define OW_NO_RESET		2

#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	0xff

uint8_t OW_Init();
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);

#endif
