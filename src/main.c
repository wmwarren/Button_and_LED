/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"


static void prv_SetupHardware(void);
static void prvSetupUart(void);
void printmsg(char*msg);

int main(void)
{

	DWT->CTRL |=(1 << 0);

	// Resets the RCC clock configuration to the default reset state.
	// Resets system clock and CPU clock to 8 MHz
	RCC_DeInit();

	// Update the system core clock variable
	SystemCoreClockUpdate();

	prv_SetupHardware();

	for(;;);



}


static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart3_init;

	memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins));
	memset(&uart3_init, 0, sizeof(uart3_init));

	//Enable UART3 Peripheral Clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	// enable alt7 mode
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &gpio_uart_pins);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9, GPIO_AF_USART3);

	// UART parameter configuration
	uart3_init.USART_BaudRate = 115200;
	uart3_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart3_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	uart3_init.USART_Parity = USART_Parity_No;
	uart3_init.USART_StopBits = USART_StopBits_1;
	uart3_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &uart3_init);

	USART_Cmd(USART3, ENABLE);
}

void prv_SetupHardware(void)
{
	prvSetupUart();
}


void printmsg(char *msg)
{
	for(uint32_t i = 0; i <strlen(msg); i++)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != SET);
		USART_SendData(USART3,msg[i]);
	}
}
