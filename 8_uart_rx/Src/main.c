#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#include "uart.h"

#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

char key;

int main(void){
	RCC->AHB1ENR |=GPIOCEN;

	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	uart2_rxtx_init();
	while(1){
		key=uart2_read();
		if (key=='1'){
				GPIOC->ODR|=LED_PIN;
		}
		else{
			GPIOC->ODR&=~LED_PIN;
		}
	}
}


