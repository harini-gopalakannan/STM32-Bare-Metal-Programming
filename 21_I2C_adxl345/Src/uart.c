#include "uart.h"

#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)

#define CR1_TE (1U<<3)
#define CR1_RE (1U<<2)
#define CR1_UE (1U<<13)

#define SR_TXE (1U<<7)
#define SR_RXNE (1U<<5)

#define CR1_RXNEIE (1U<<5)

/*--------------DMA----------------*/

#define DMA1EN (1U<<21)
#define DMA_S6_EN (1U<<0)
#define CHSEL4 (1U<<27)
#define DMA_MEM_INC (1U<<10)
#define DMA_DIR_MEM_TO_PERIPH (1U<<6)
#define DMA_CR_TCIE (1U<<4)

#define USART_CR3_DMAT (1U<<7)
/*---------------------------------*/


#define SYSFREQ 16000000
#define APB1_CLK SYSFREQ

#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
void uart2_tx_init(void);
void uart2_write(int ch);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len)
{
	/*enable clock access to dma - connect to ahb1*/
	RCC->AHB1ENR |= DMA1EN;
	/*disable dma1 stream6*/
	DMA1_Stream6 ->CR &=~ DMA_S6_EN;
	/*wait until sma1 stream6 is disabled*/
	while (DMA1_Stream6 ->CR & DMA_S6_EN){};
	/*clear all interrupt flags of stream6*/
	DMA1->HIFCR |=(1U<<16);
	DMA1->HIFCR |=(1U<<18);
	DMA1->HIFCR |=(1U<<19);
	DMA1->HIFCR |=(1U<<20);
	DMA1->HIFCR |=(1U<<21);
	/*set the destination buffer*/
	DMA1_Stream6->PAR =dst;
	/*set the source buffer*/
	DMA1_Stream6->M0AR = src;
	/*set the length*/
	DMA1_Stream6->NDTR = len;
	/*select STREAM6 AND CHANNEL 4*/
	DMA1_Stream6 ->CR = CHSEL4;
	/*enable memory to increment*/
	DMA1_Stream6 ->CR |= DMA_MEM_INC;
	/*configure the transfer direction (TX) - here, data moving from memory to peripheral*/
	DMA1_Stream6 ->CR |= DMA_DIR_MEM_TO_PERIPH;
	/*ENABLE DMA TX COMPLETE INTERRUPT*/
	DMA1_Stream6 ->CR |= DMA_CR_TCIE;
	/*enable direct mode, disable FIFO*/
	DMA1_Stream6 ->FCR = 0;

	/*enable DMA1 STREAM6*/
	DMA1_Stream6 ->CR |= DMA_S6_EN;
	/*enable UART2_TX dma*/
	USART2->CR3 |= USART_CR3_DMAT;

	/*DMA interrupt enable in NVIC - so that we can raise interrupt/ run this if dma transfer is
	 * half complete, and run this other code if its 3/4th complete, and raise this other
	 * interrupt if dma transfer is fully complete- just examples
	 */
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void uart2_rxtx_init(void){
	/***Configure uart gpio pin****/

	/**1.Enable clock access to gpioa***/
	RCC->AHB1ENR|=GPIOAEN;

	/***2.Set PA2 mode to AF mode ****/
	GPIOA->MODER&=~(1U<<4);
	GPIOA->MODER|=(1U<<5);

	/**set pa3 to af mode**/
	GPIOA->MODER&=~(1U<<6);
	GPIOA->MODER|=(1U<<7);

	/***2.Set PA2 AF type to UART_TX(AF07)****/
	/**GPIO AFLR - ref manual - Pin2 -bit 8 to 11**/
	GPIOA->AFR[0]|=(1U<<8);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);

	/**set pa3 af to uart_rx**/
	GPIOA->AFR[0]|=(1U<<12);
	GPIOA->AFR[0]|=(1U<<13);
	GPIOA->AFR[0]|=(1U<<14);
	GPIOA->AFR[0]&=~(1U<<15);

	/*****Configure UART module ********/
	/**Enable clock access to uart2****/
	RCC->APB1ENR|=UART2EN;
	/***configure baudrate***/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	/***configure the transfer direction***/
	USART2->CR1 = (CR1_RE|CR1_TE); //Not using | operate deliberately, clearing all bits except bit 3


	/***enable the UART module***/
	USART2->CR1|= CR1_UE;
}


void uart2_rx_interrupt_init(void){
	/***Configure uart gpio pin****/

	/**1.Enable clock access to gpioa***/
	RCC->AHB1ENR|=GPIOAEN;

	/***2.Set PA2 mode to AF mode ****/
	GPIOA->MODER&=~(1U<<4);
	GPIOA->MODER|=(1U<<5);

	/**set pa3 to af mode**/
	GPIOA->MODER&=~(1U<<6);
	GPIOA->MODER|=(1U<<7);

	/***2.Set PA2 AF type to UART_TX(AF07)****/
	/**GPIO AFLR - ref manual - Pin2 -bit 8 to 11**/
	GPIOA->AFR[0]|=(1U<<8);
	GPIOA->AFR[0]|=(1U<<9);
	GPIOA->AFR[0]|=(1U<<10);
	GPIOA->AFR[0]&=~(1U<<11);

	/**set pa3 af to uart_rx**/
	GPIOA->AFR[0]|=(1U<<12);
	GPIOA->AFR[0]|=(1U<<13);
	GPIOA->AFR[0]|=(1U<<14);
	GPIOA->AFR[0]&=~(1U<<15);

	/*****Configure UART module ********/
	/**Enable clock access to uart2****/
	RCC->APB1ENR|=UART2EN;
	/***configure baudrate***/
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	/***configure the transfer direction***/
	USART2->CR1 = (CR1_RE|CR1_TE); //Not using | operate deliberately, clearing all bits except bit 3

	/*ENABLE rxne INTERRUPT*/
	USART2->CR1 |=CR1_RXNEIE;

	/*Enable UART2 interrupt in NVIC*/
	NVIC_EnableIRQ(USART2_IRQn);

	/***enable the UART module***/
	USART2->CR1|= CR1_UE;
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

char uart2_read(void){
	/**make sure receive data register is not empty*/
	while (!(USART2->SR & SR_RXNE)){}

	return USART2->DR;
}

void uart2_write(int ch){
	/***MAke sure TX register is empty b4 putting data in it***/
	while (!(USART2->SR & SR_TXE)){}
	/**Write to transmit data register***/
	USART2->DR=(ch & 0xFF);
}



static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate){
	USARTx->BRR=compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate){
	return ((PeriphClk +(BaudRate/2U))/BaudRate);
}
