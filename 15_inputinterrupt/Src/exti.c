/*PUSH BUTTON is connected to PA0*/

#include "exti.h"

#define GPIOAEN (1U<<0)
#define SYSCFGEN (1U<<14)

void pa0_exti_init(void){
	/*disable global interrupt: GOOD PRACTICE
	 * intrinsic function that comes with CortexM.h file*/
	__disable_irq();

	/*enable clock access for gpioc*/
	RCC->AHB1ENR |= GPIOAEN;
	/*set pa5 as input pin*/
	GPIOA->MODER &=~(1U<<0);
	GPIOA->MODER &=~(1U<<1);

	/* enable pull-up for PA0 */
	GPIOA->PUPDR &= ~(1U << 0);   // clear PUPDR0
	GPIOA->PUPDR &= ~(1U << 1);
	GPIOA->PUPDR |=  (1U << 0);   // PU = 01

	/*enable clock access to SYScfg ->connected to APB2*/
	RCC->APB2ENR |= SYSCFGEN;
	/*select porta for exti0 */
	SYSCFG->EXTICR[0] &=~ 0x000F;
	/*unmask exti0 */
	EXTI->IMR |= (1U<<0);
	/*select falling edge trigger*/
	EXTI->FTSR |= (1U<<0);
	/*enable exti line in NVIC - Function that comes with coreM.h file*/
	NVIC_EnableIRQ(EXTI0_IRQn);
	/*enable global interrupt*/
	__enable_irq();
}
