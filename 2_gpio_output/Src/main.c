#include "stm32f4xx.h"

#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

int main(void){
	RCC->AHB1ENR |=GPIOCEN;

	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	while(1){
		GPIOC->ODR^=LED_PIN;
		for (int i=0; i<100000; i++){
		}
	}
}
