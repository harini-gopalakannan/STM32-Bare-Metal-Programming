#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#include "uart.h"


int main(void){

	uart2_tx_init();
	while(1){
		printf("Hello World ....\n");
	}
}


