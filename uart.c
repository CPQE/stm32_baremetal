#include <stdint.h>
#include "uart.h"

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart_write(int ch); 

void uart_init(void){
	RCC->AHB1ENR |= GPIOAEN; // enable clock access to GPIOA
	GPIOA->MODER &= ~(1U << 4); //set mode of PA2 to alternate function mode
	GPIOA->MODER |= (1U << 5); 
	//set alternate function type of Pin A2 (AFR[0]) to type AF7 (0111), which means UART2_TX.
	GPIOA->AFR[0] |= (1U << 8); 
	GPIOA->AFR[0] |= (1U << 9); 
	GPIOA->AFR[0] |= (1U << 10); 
	GPIOA->AFR[0] &= ~(1U << 11); 

	//enable clock access to UART2
	RCC->APB1ENR |= UART2EN; 
	//configure uart baudrate 
	uart_set_baudrate(APB1_CLK, DBG_UART_BAUDRATE); 
	USART2->CR1 = CR1_TE;
	USART2->CR1 |= CR1_UE; 
}

void uart_print(const char *str) {
    while (*str) {
        uart_write(*str++);
    }
}

static void uart_write(int ch){
	while (!(USART2->SR & SR_TXE)){}
	USART2->DR = (ch & 0xFF);  //write to transmit data register
}



static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate){
	return ((periph_clk + (baudrate/2U)) / baudrate); 
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
	USART2->BRR = compute_uart_bd(periph_clk, baudrate); 
}




































