#include "gpio.h"

#define GPIOAEN (1U << 0) /* GPIO A in AHB1ENR register */
#define GPIOCEN (1U<<2) /* GPIO C in AHB1ENR register */

#define BTN_PIN (1U<<13) /* enable the User blue button */
#define LED_BS5 (1U << 5) /*Bit Set Pin 5*/
#define LED_BR5 (1U << 21) /*Bit Reset Pin 5 */

#define LED_PIN (1U<<5)

void button_init(void){
	/*Enable clock access to PORTC */
	RCC->AHB1ENR |= GPIOCEN; 
	/*Set PC13 as an input pin (00 is input, 01 is output, 10 is alt, 11 is analog) */
	GPIOC->MODER &= ~(1U<<26); 
	GPIOC->MODER &= ~(1U<<27); 
}

void led_init(void){
	/* Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;
	/*Set PA5 mode to output mode */
	GPIOA->MODER |= (1U << 10); 
	GPIOA->MODER &= ~(1U<<11); 
}

void led_toggle(void){
	/* Toggle PA5*/
	GPIOA->ODR ^= LED_PIN; 
}

void led_on(void){
	GPIOA->BSRR |= LED_BS5;  //turn on the led by toggling BSRR[5]
}

void led_off(void){
	GPIOA->BSRR |= LED_BR5; //turn off the led by toggling BSRR[21]
}

bool get_btn_state(void){
	/*BTN is active low*/

	/* Check if IDR register's 13th index is toggled off (==1) */
	if (GPIOC->IDR & BTN_PIN){
		return false;
	}
	else{
		return true; 
	}
}



