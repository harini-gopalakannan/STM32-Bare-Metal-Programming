#include "stm32f4xx.h"

#define tim2_en (1U<<0)
#define CR1_CEN (1U<<0)
#define OC_TOGGLE (1U<<4)|(1U<<5)
#define OC_EN (1U<<0)

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

void tim2_pa5_output_compare(void){

	/*enable clock to gpioa*/
	RCC->AHB1ENR |= (1U<<0);
	/*set pa5 to AF mpde*/
	GPIOA->MODER &=~(1U<<10);
	GPIOA->MODER |= (1U<<11);
	/*set pa5 AF type as tim2 ch1 == AF01*/
	GPIOA->AFR[0] |= (1U<<20);

	/*enable clock access to tim2 thru apb1*/
	RCC->APB1ENR |= tim2_en;
	/*set PSC value: configuring clock tree, essentially, so that timer is 1hz or 1s
	 * so every 1s there is a timeout*/
	TIM2->PSC =1600-1;  //16Mhz /1600 = 10 000
	/*set ARR value	 */
	TIM2->ARR = 10000 -1;  // 10000/10000 = 1 s

	/*set output compare toggle mode*/
	TIM2->CCMR1 = OC_TOGGLE;
	/*enable tim2 channel 1 in compare mode*/
	TIM2->CCER = OC_EN;

	/*clear counter*/
	TIM2->CNT = 0;
	/*enable timer*/
	TIM2->CR1 = CR1_CEN;
}
