#include "spi.h"
#define SPI1EN (1U<<12) //used to enable clock for SPI1 peripheral
#define GPIOAEN (1U<<0) //enables clock for GPIOA
#define SR_TXE (1U<<1) //shows that tx buffer empty

#define SR_RXNE (1U<<0)//shows rx buffer not empty
#define SR_BSY (1U<<7)  //SPI interface busy with a transfer

void spi_gpio_init(void){
    RCC->AHB1ENR |= GPIOAEN;

    // PA5, PA6, PA7 → AF5
    GPIOA->MODER &= ~( (3U<<10) | (3U<<12) | (3U<<14) );
    GPIOA->MODER |=  ( (2U<<10) | (2U<<12) | (2U<<14) );

    GPIOA->AFR[0] &= ~(0xFFF << 20);
    GPIOA->AFR[0] |=  (0x555 << 20);

    // PA9 → output
    GPIOA->MODER &= ~(3U<<18);
    GPIOA->MODER |=  (1U<<18);
    GPIOA->ODR |= (1U<<9);
}

void spi1_config(void){
	RCC->APB2ENR |= SPI1EN; //enable SPI1's Clock (RM0383 6.3.12, p. 121)
	// set clock to fPCLK/4 (001), 
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
	for(uint32_t i = 0; i < size; i++){
		//wait until TXE set (nothing left to transmit from previous calls)
		while( ! (SPI1->SR & (SR_TXE) )) {}
		//write data to data register
		SPI1->DR = data[i];
	}
	//wait until TXE set (nothing left to transmit)
	while(!(SPI1->SR & (SR_TXE) )){}
	//wait for BUSY flag to reset (don't clear flags and exit until transmission done)
	while((SPI1->SR & (SR_BSY) )) {}

	//clear OVR flag by doing 'software operation'
	// temp = SPI1->DR;
	// temp = SPI1->SR; 
    (void)SPI1->DR;
    (void)SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t size){
	for(uint32_t i = 0; i < size; i++){ //every byte transmitted one at a time
		while(!(SPI1->SR & SR_TXE)){}
		SPI1->DR = 0; //dummy byte to generate clock
		while(!(SPI1->SR & (SR_RXNE))){}
		*data++ = (SPI1->DR); 
		//wait for BUSY flag to reset (don't clear flags and exit until transmission done)
		while((SPI1->SR & (SR_BSY) )) {}
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

