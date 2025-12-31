#include "stm32f4xx.h"

#define tim2_en (1U<<0)
#define CR1_CEN (1U<<0)
#define DIER_UIE (1U<<0)
void tim2_1hz_init(void){
	/*enable clock access to tim2 thru apb1*/
	RCC->APB1ENR |= tim2_en;
	/*set PSC value: configuring clock tree, essentially, so that timer is 1hz or 1s
	 * so every 1s there is a timeout*/
	TIM2->PSC =1600-1;  //16Mhz /1600 = 10 000
	/*set ARR value	 */
	TIM2->ARR = 10000 -1;  // 10000/10000 = 1 s
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 = CR1_CEN;
}

void tim2_1hz_interrupt_init(void)
{
	/*enable clock access to tim2 thru apb1*/
	RCC->APB1ENR |= tim2_en;
	/*set PSC value: configuring clock tree, essentially, so that timer is 1hz or 1s
	 * so every 1s there is a timeout*/
	TIM2->PSC =1600-1;  //16Mhz /1600 = 10 000
	/*set ARR value	 */
	TIM2->ARR = 10000 -1;  // 10000/10000 = 1 s
	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 = CR1_CEN;

	/*enable tim interrupt - UIE -> TIMEOUT HAS OCCURED*/
	TIM2->DIER |=DIER_UIE;
	/*enable tim interrupt in NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);
}
