#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6
#include "uart.h"
#include "systick.h"
#include "tim.h"



int main(void){
	tim2_pa5_output_compare();
	while(1){
	}
}


