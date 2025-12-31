#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6
#include "uart.h"
#include "systick.h"

#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

void systick_callback(void);

int main(void){
	RCC->AHB1ENR |=GPIOCEN;

	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	uart2_tx_init();
	systick_1hz_interrupt();

	while(1){
			}
}

void systick_callback(void){
	printf("A second passed!! \n\r");
	GPIOC->ODR^=LED_PIN;

}

void SysTick_Handler(void){
	systick_callback();
}



