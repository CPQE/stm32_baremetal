#include "tim.h"

#define TIM2EN (1U<<0)  /*enables the clock for TIM2 by settin 1st bit of APB1ENR*/
#define CR1_CEN (1U<<0) /*enables counter in TIM2 Control register 1 */

void tim2_1hz_init(void){
	/* enable clock access to timer 1*/
	RCC->APB1ENR |= TIM2EN;
	/*Set prescaler value */
	TIM2->PSC = 1600 - 1; 
	/* set auto-reload value */
	TIM2->ARR = 10000 - 1; 
	/* clear counter */
	TIM2->CNT = 0; 
	/* Enable the timer so it starts counting up to 10000*/
	TIM2->CR1 = CR1_CEN; 
}

