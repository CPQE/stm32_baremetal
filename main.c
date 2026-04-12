#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "gpio.h"
// #include "systick.h"
// #include "tim.h"
#include "uart.h"

#include "adxl345.h"
#include "spi.h"
int16_t accel_x, accel_y, accel_z;
float accel_x_g, accel_y_g, accel_z_g; 
uint8_t data_buffer[6]; 
// bool btn_state; //global variable with external linkage

int main(void){
	uart_init(); 
	adxl_init(); 
	while(1){
		adxl_read(ADXL345_REG_DATA_START, data_buffer); 
		accel_x = (int16_t) ((data_buffer[1] << 8) | data_buffer[0]); 
		accel_y = (int16_t)((data_buffer[3] << 8) | data_buffer[2]); 
		accel_z = (int16_t) ((data_buffer[5] << 8) | data_buffer[4]); 

		//conver raw data to g values
		accel_x_g = accel_x * 0.078f;
		accel_y_g = accel_y * 0.078f;
		accel_z_g = accel_z * 0.078f;
		//print values ofr debuggin purposes
		uart_print("accel_x: ");
		uart_print_float(accel_x_g);
		uart_print("\r\n");

		uart_print("accel_y: ");
		uart_print_float(accel_y_g);
		uart_print("\r\n");

		uart_print("accel_z: ");
		uart_print_float(accel_z_g);
		uart_print("\r\n");

	}
	return 0; 
}



	// uart_init(); 
	// uint32_t seq_num = 0; 
	// while(1){
	// 	uart_print("Hi from cyrus!\r\n"); 
	// }
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