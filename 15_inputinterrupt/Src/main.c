#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6
#include "uart.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

static void exti_callback(void);
void EXTI0_IRQHandler(void);

int main(void){
	RCC->AHB1ENR |=GPIOCEN;

	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	pa0_exti_init();
	uart2_tx_init();

	while(1){

	}
}

static void exti_callback(void){
	printf("BTN Pressed....\n\r");
	GPIOC->ODR ^=LED_PIN;
}

void EXTI0_IRQHandler(void){
	if ((EXTI->PR & LINE0)!=0){
		/* CLEAR PR FLAG*/
		EXTI->PR |=LINE0;
		/*DO SMTH*/
		exti_callback();
	}
}

