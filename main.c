#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "gpio.h"
// #include "systick.h"
// #include "tim.h"
#include "uart.h"
#include "stm32f4xx.h"
#include "adxl345.h"
#include "spi.h"
int16_t accel_x, accel_y, accel_z; //store raw accel data
float accel_x_g, accel_y_g, accel_z_g; //store converted acceleromter data in g units
uint8_t data_buffer[6]; //holds raw data bytes to read from ADXL345
// bool btn_state; //global variable with external linkage

int main(void){
	SCB->CPACR |= (0xF << 20); //needed to enable FPU on f411re.
	// Confirm in ARM Cortex M4 Technical Reference Manual (TRM) on page 71, section 7.3.2
	uart_init(); 
	adxl_init();  //initialize the accelerometer
	uart_print("+/-4G  X,Y,Z: ");
	while(1){
		//reads 6 bytes of data starting at 0x32, DATAX0.
		// This gives X0,X1, Y0,Y1, Z0,Z1 data all in 1 chunk and stuffs them 
		//into data_buffer.
		adxl_read(ADXL345_REG_DATA_START, data_buffer);  
		/*
		Each axis needs 16 bits to represent the acceleration value.
		But the ADXL345 only has 8-bit registers, so it splits each 16-bit value across 
		2 consecutive registers:
		X-axis:
		    Register 0x32 = X_LSB (lower 8 bits) (X0 in datasheet) 
		    Register 0x33 = X_MSB (upper 8 bits) (X1 in datasheet) 

		Y-axis:
		    Register 0x34 = Y_LSB  (Y0 in datasheet) 
		    Register 0x35 = Y_MSB  (Y1 in datasheet) 

		Z-axis:
		    Register 0x36 = Z_LSB (Z0 in datasheet)
		    Register 0x37 = Z_MSB (Z1 in datasheet)
		When you do multi-byte read starting at 0x32, you get all 6 bytes in one SPI transaction
		*/
		accel_x = (int16_t) ((data_buffer[1] << 8) | data_buffer[0]); 
		accel_y = (int16_t)((data_buffer[3] << 8) | data_buffer[2]); 
		accel_z = (int16_t) ((data_buffer[5] << 8) | data_buffer[4]); 

		//conver raw data to g values
		accel_x_g = accel_x * 0.0039f;
		accel_y_g = accel_y * 0.0039f;
		accel_z_g = accel_z * 0.0039f;
		// int32_t accel_x_mg = accel_x * 4;   // 4 mg/LSB
		// int32_t accel_y_mg = accel_y * 4;   // 4 mg/LSB
		// int32_t accel_z_mg = accel_z * 4;   // 4 mg/LSB


		uart_print_float(accel_x_g);
		uart_print(",");
		uart_print_float(accel_y_g);
		uart_print(",");
		uart_print_float(accel_z_g);
		uart_print("\r\n");

		// Delay ~100ms between reads
		for (volatile int i = 0; i < 20000; i++){}   // slower loop
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