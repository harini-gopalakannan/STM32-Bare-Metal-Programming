#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6
#include "uart.h"
#include "systick.h"
#include "tim.h"

int timestamp=0;
/*setup: connect wire from pa5 to pa6*/
int main(void){
	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1){
		/*wait until edge is captured*/
		while (!(TIM3->SR & SR_CC1IF)){}

		/*READ VALUE*/
		timestamp=TIM3->CCR1;
	}
}


