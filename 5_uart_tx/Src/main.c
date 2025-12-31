#include "stm32f4xx.h"
#include <stdint.h>
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)

#define CR1_TE (1U<<3)
#define CR1_UE (1U<<13)

#define SR_TXE (1U<<7)

#define SYSFREQ 16000000
#define APB1_CLK SYSFREQ

#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_tx_init(void);
void uart2_write(int ch);

int main(void){

	uart2_tx_init();

	while(1){
		uart2_write('Y');
	}
}

void uart2_write(int ch){
	/***MAke sure TX register is empty b4 putting data in it***/
	while (!(USART2->SR & SR_TXE)){}
	/**Write to transmit data register***/
	USART2->DR=(ch & 0xFF);
}

void uart2_tx_init(void){
	/***Configure uart gpio pin****/

	/**1.Enable clock access to gpioa***/
	RCC->AHB1ENR|=GPIOAEN;

	/***2.Set PA2 mode to AF mode ****/
	GPIOA->MODER&=~(1U<<4);
	GPIOA->MODER|=(1U<<5);

	/***2.Set PA2 AF type to UART_TX(AF07)****/
	/**GPIO AFLR - ref manual - Pin2 -bit 8 to 11**/
	GPIOA->AFR[0]|=(1U<<8);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);



	/*****Configure UART module ********/
	/**Enable clock access to uart2****/
	RCC->APB1ENR|=UART2EN;
	/***configure baudrate***/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	/***configure the transfer direction***/
	USART2->CR1 = CR1_TE; //Not using | operate deliberately, clearing all bits except bit 3
	/***enable the UART module***/
	USART2->CR1|= CR1_UE;
}

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR=compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return ((PeriphClk +(BaudRate/2U))/BaudRate);
}
