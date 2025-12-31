#include "stm32f4xx.h"



#define GPIOCEN (1U<<2)
#define PIN13 (1U<<13)
#define LED_PIN PIN13

#define GPIOAEN	(1U<<0)
#define PIN0 (1U<<0)
#define BTN_PIN	PIN0



int main(void)
{

	/*Enable clock access to GPIOA and GPIOC*/
	RCC->AHB1ENR |=GPIOAEN;
	RCC->AHB1ENR |=GPIOCEN;

    /*Set PC13 as output pin*/
	GPIOC->MODER |= (1U<<26);
	GPIOC->MODER &=~(1U<<27);

	/*Set PA0 as input pin*/
	/* Set PA0 as input */
	GPIOA->MODER &= ~(1U << 0);
	GPIOA->MODER &= ~(1U << 1);

	/* Pull-down for USER button */
	GPIOA->PUPDR &= ~(1U << 1);
	GPIOA->PUPDR |=  (1U << 0);


	while(1)
	{

		/*Check if BTN is pressed*/
		if (!(GPIOA->IDR & BTN_PIN))
		{
			/*Turn on led*/
			GPIOC->BSRR = (1U<<29);
		}
		else{
			/*Turn off led*/
			GPIOC->BSRR = LED_PIN;

		}

	}
}
