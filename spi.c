#include "spi.h"

#define SPI1EN (1U<<12)
#define GPIOAEN (1U<<0)
#define SR_TXE (1U<<1)
#define SR_RXNE (1U<<0)
#define SR_BSY (1U<<7)

void spi_gpio_init(void){
	RCC->AHB1ENR |= GPIOAEN; 
	/* Set PA5, PA6, PA7 mode to alternate functions */

	//PA5
	GPIOA->MODER &=~(1U<<10); 
	GPIOA->MODER |= (1U<<11); 
	//PA6
	GPIOA->MODER &=~(1U<<12); 
	GPIOA->MODER |= (1U<<13); 
	//PA7
	GPIOA->MODER &= ~(1U<<14); 
	GPIOA->MODER |= (1U<<15); 
	//PA9
	GPIOA->MODER |= (1U<<18); 
	GPIOA->MODER &= ~(1U<<19); 

	//set PA5, PA6,PA7 alt func type to SPI1
	GPIOA->AFR[0] |= (1U<<20); 
	GPIOA->AFR[0] &= ~(1U<<21); 
	GPIOA->AFR[0] |= (1U<<22); 
	GPIOA->AFR[0] &= ~(1U<<23); 
}