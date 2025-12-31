/*
 * adc.h
 *
 *  Created on: 04-Dec-2025
 *      Author: harini gopalakannan
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>
void pa1_adc_init(void);

void dma2_stream0_init();

#define LISR_TCIF0 (1U<<5)
#define LIFCR_CTCIF0 (1U<<5)

#endif /* ADC_H_ */
