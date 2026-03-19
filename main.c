#include <stdint.h>
#include "gpio.h"
#include "systick.h"

bool btn_state; //global variable with external linkage

int main(void){
	led_init();

	while(1){
		/* delay for 500ms using systick */
		systick_msec_delay(500); 
		/* Toggle the LED */
		led_toggle();
	}
	return 0; 
}


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