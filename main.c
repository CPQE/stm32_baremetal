#include <stdint.h>
#include <stdio.h>
#include "gpio.h"
#include "systick.h"
#include "tim.h"
#include "uart.h"

// bool btn_state; //global variable with external linkage


int main(void){
	uart_init(); 
	uint32_t seq_num = 0; 
	while(1){
		uart_print("Hi from cyrus!\r\n"); 
	}
	return 0; 
}

// led_init();
// tim2_1hz_init(); 
// led_toggle(); 
// 		/* Wait for UIF, aka when Timer2 counts down to 0 */
// 		while (!(TIM2->SR & SR_UIF)){}
// 		/* Clear UIF so can be used for next countdown */
// 		TIM2->SR &= ~SR_UIF; 
// button_init();
// btn_state = get_btn_state(); 
// if(btn_state){
		// 	led_on(); 
		// }
		// else{
		// 	led_off(); 
		// // 
		// led_on(); 

		// for(volatile uint32_t i = 0; i < 800000; i++){}
		// led_off(); 
		// for(volatile uint32_t i = 0; i < 800000; i++){}
// }
// 