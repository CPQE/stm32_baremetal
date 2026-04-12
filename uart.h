#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define GPIOAEN (1U << 0) //enable clock for GPIO A by setting bit 0 in AHB1ENR register
#define UART2EN (1U << 17) //enables clock for UART2 by setting bit 17 in the APB1ENR register

#define DBG_UART_BAUDRATE 115200 //baud rate at 115200 bps
#define SYS_FREQ 16000000     //system clock runs at 16MHz
#define APB1_CLK SYS_FREQ
#define CR1_TE (1U << 3) //enables tranmsitt
#define CR1_UE (1U << 13)
#define SR_TXE (1U << 7)

void uart_print(const char *str); 
void uart_init(void);
void uart_print_int(int value);
void uart_print_label(const char *label, int value);
void uart_print_float(float value);
#endif 