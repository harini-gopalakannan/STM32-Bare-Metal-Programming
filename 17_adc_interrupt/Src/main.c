#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "adc.h"
//FOLLOWING CODE HAS BEEN WRITTE FOR STM32F411CEU6

#include "uart.h"

uint32_t sensor_val;

static void adc_callback(void);

int main(void){

	uart2_tx_init();
	pa1_adc_interrupt_init();
	start_conversion();
	while(1){


	}
}

static void adc_callback(void)
{
	sensor_val=ADC1->DR;
	printf("Sensor value: %d \n\r",(int)sensor_val);

}

void ADC_IRQHandler(void){
	/*check if eoc  in SR is set*/
	if ((ADC1->SR & SR_EOC)!=0){
		/*clear EOC		 */
		ADC1->SR &=~SR_EOC;

		//do smth
		adc_callback();
	}

}

