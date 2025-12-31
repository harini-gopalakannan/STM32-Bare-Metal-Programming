/*
 * uart.h
 *
 *  Created on: 04-Dec-2025
 *      Author: harini gopalakannan
 */

#ifndef UART_H_
#define UART_H_
#include "stm32f4xx.h"
#include <stdint.h>

void uart2_tx_init(void);
void uart2_write(int ch);

#endif /* UART_H_ */
