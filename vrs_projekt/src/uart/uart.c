#include "uart/uart.h"

volatile char buffer[100];
volatile int indexBuffer = 0;
volatile int pocetZnakov = 0;
volatile char nextZnak = 0;


void initUART(void) {
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */       //turning on the needed peripherals
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//choosing peripherals for selected pins
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//usart configuration
	USART_InitStructure.USART_BaudRate = 9600 * 2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_Init(USART3, &USART_InitStructure);

	//configuring interrupts
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//choosing which event should cause interrupt
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	/* Enable USART */
	USART_Cmd(USART3, ENABLE);

}

void sendCharUART(char znak) {
	USART_SendData(USART3, (uint8_t) znak);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		;
}

void sendStringUART(char znak[], int length) {
	for (int i = 0; i < length; i++) {
		sendCharUART(znak[i]);
	}
}

void sendCharByBufferUART(char znak) {
	if (pocetZnakov < 100) {
		buffer[pocetZnakov++] = znak;
	}
}

void sendStringByBufferUART(char znak[], int length) {
	if (pocetZnakov == 0 && length >= 1) {
		for (int i = 1; i < length; i++) {
			sendCharByBufferUART(znak[i]);
		}
		USART_SendData(USART3, (uint8_t) znak[0]);
	} else {
		for (int i = 0; i < length; i++) {
			sendCharByBufferUART(znak[i]);
		}
	}
}

void callbackTXUART() {
	if (pocetZnakov > indexBuffer) {
		indexBuffer++;
		nextZnak = 1;
		if (indexBuffer >= 100) {
			indexBuffer = 0;
			pocetZnakov = 0;
		}
	} else {
		indexBuffer = 0;
		pocetZnakov = 0;
	}
	if (nextZnak == 1) {
		USART_SendData(USART3, buffer[indexBuffer]);
		nextZnak = 0;
	}
}
