#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#include "uart.h"

uint32_t sensor_val;

int main(void){

	uart2_tx_init();
	pa1_adc_init();
	start_conversion();
	while(1){

		sensor_val=adc_read();
		printf("Sensor value: %d \n\r",(int)sensor_val);
	}
}


