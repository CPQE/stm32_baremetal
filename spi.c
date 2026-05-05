#include "spi.h"
#define SPI1EN (1U<<12) //used to enable clock for SPI1 peripheral
#define GPIOAEN (1U<<0) //enables clock for GPIOA
#define SR_TXE (1U<<1) //shows that tx buffer empty

#define SR_RXNE (1U<<0)//shows rx buffer not empty
#define SR_BSY (1U<<7)  //SPI interface busy with a transfer

void spi_gpio_init(void){
	//set pin A5-7 to alternate function (01)
	RCC->AHB1ENR |= GPIOAEN;
	GPIOA->MODER &= ~(1U<<10);
	GPIOA->MODER |= (1U<<11); 
	GPIOA->MODER &= ~(1U<<12); 
	GPIOA->MODER |= (1U<<13); 
	GPIOA->MODER &= ~(1U<<14); 
	GPIOA->MODER |= (1U<<15); 
	GPIOA->MODER &= ~(1U<<18); 
	GPIOA->MODER |= (1U<<19); 

	GPIOA->AFR[0] |= (1U<<20); 
	GPIOA->AFR[0] &= ~(1U<<21); 

	GPIOA->AFR[0] |= (1U<<22); 
	GPIOA->AFR[0] &= ~(1U<<23); 

	GPIOA->AFR[0] |= (1U<<24); 
	GPIOA->AFR[0] &= ~(1U<<25); 
	GPIOA->AFR[0] |= (1U<<26); 
	GPIOA->AFR[0] &= ~(1U<<27); 

	GPIOA->AFR[0] |= (1U<<28); 
	GPIOA->AFR[0] &= ~(1U<<29); 
	GPIOA->AFR[0] |= (1U<<30); 
	GPIOA->AFR[0] &= ~(1U<<31); 
}

void spi1_config(void){
	RCC->APB2ENR |= SPI1EN; 
	// set clock to fPCLK/4
	SPI1->CR1 |= (1U<<3); 
	SPI1->CR1 &= ~(1U<<4); 
	SPI1->CR1 &= ~(1U<<5); 
	//set CPOL to 1 and CPHA to 1
	SPI1->CR1 |= (1U<<0); 
	SPI1->CR1 |= (1U<<1); 
	//enable full duplex
	SPI1->CR1 &= ~(1U<<10); 
	//set MSB first (BIG ENDIAN)
	SPI1->CR1 &= ~(1U<<7); 
	//set mode to master
	SPI1->CR1 |=(1U<<2); 
	//Set 8 bit data mode
	SPI1->CR1 &= ~(1U<<11); 
	//select software slave management by setting SSM=1 and SSI = 1
	SPI1->CR1 |= (1<<8);
	SPI1->CR1 |= (1<<9);
	//enable SPI module
	SPI1->CR1 |= (1<<6); 
}

void spi1_transmit(uint8_t *data, uint32_t size){
	uint32_t i = 0; 
	uint8_t temp; 
	while(i < size){
		//wait until TXE set (nothing left to transmit from previous calls)
		while( ! (SPI1->SR & (SR_TXE) )) {}
		//write data to data register
		SPI1->DR = data[i];
	    i++; 
	}
	//wait until TXE set
	while(!(SPI1->SR & (SR_TXE) )){}
    //wait for BUSY flag to reset
	while((SPI1->SR & (SR_BSY) )) {}
	//clear OVR flag by doing 'software operation'
	temp = SPI1->DR;
	temp = SPI1->SR; 
}

void spi1_receive(uint8_t *data, uint32_t size){
	while(size){ //every byte transmitted one at a time
		SPI1->DR = 0; 
		while(!(SPI1->SR & (SR_RXNE))){}
		*data++ = (SPI1->DR); 
		size--; 
	}
}
//control the CS pin, enable slave device
void cs_enable(void){
	GPIOA->ODR &= ~(1U<<9); 
}

//pull high to disable slave device
void cs_disable(void){
	GPIOA->ODR |= (1U<<9); 
}

