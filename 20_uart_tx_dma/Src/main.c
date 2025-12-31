#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#include "uart.h"

#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

char key;
static void dma_callback(void);

int main(void){

	char message [31] = "Hello from Stm32 DMA transfer\n\r";
	RCC->AHB1ENR |=GPIOCEN;

	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	uart2_tx_init();
	dma1_stream6_init((uint32_t) message, (uint32_t) &USART2->DR, 31);

	while(1){
	}
}

static void dma_callback(void){
	GPIOC->ODR |= LED_PIN;
}

void DMA1_Stream6_IRQHandler(void)
{
	/*CHECK FOR TRASNFER COMPLETE INTERRUPT*/
	if (DMA1->HISR & HISR_TCIF6){
		/*CLEAR FLAG*/
		DMA1->HIFCR |= HIFCR_CTCIF6;
		//DO SMTH
		dma_callback();
	}
}


