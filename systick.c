#include "systick.h" 

#define CTRL_ENABLE (1U<<0)  //used to enable systick timer
#define CTRL_CLCKSRC (1U<<2) //used to select internal clock source for systick timer
#define CTRL_COUNTFLAG (1U<<16) //checks COUNTFLAG bit which sees when timer has counted to 0

/* default frequency of MCU is 16MHz, and want to have systick over 1ms 
	so we define loading time as 16e6 * 1e-3 = 16e3 Hz */
#define ONE_MSEC_LOAD 16000

/*
	SYST_CSR <-> SysTick->CTRL, Control and Status Register, controls timer op and gives status info. 
		SysTick->CTRL[0], ENABLE,  0 or 1, enables/disables systick counter
		SysTick->CTRL[1], TICKINT, Enables/disables systick interrupt 
		SysTick->CTRL[2], CLKSOURCE, selects clock source, 0 is external clock source and 1 is processor clock
		SysTick->CTRL[15],COUNTFLAG, says whether counter has reached zero since last read(1 yes, 0 no)

	SYST_RVR <-> SysTick->LOAD, Reload value register, 24-bit (0x0-0x00FFFFFF) value that says what to initialize timer with
	SYST_CVR <-> SysTick->VAL, Current value register, holds value of the counter. counts down based on CTRL
	SYST_CALIB <-> SysTick->CALIB (used to hold these new mapping of variables)
*/

void systick_msec_delay(uint32_t delay) {
	SysTick->LOAD = ONE_MSEC_LOAD - 1;  //loads systick timer with number of clock cycles for 1ms
	/*Clear systick current value register */
	SysTick->VAL = 0;  //clears current value register to reset timer 
	/*Select internal clock source */
	SysTick->CTRL = CTRL_CLCKSRC; //selects internal clock source
	/*Enable systick*/
	SysTick->CTRL |= CTRL_ENABLE;  //enables processor clock as the clock source
	for (uint32_t i = 0; i < delay; i++) {
		while( (SysTick->CTRL & CTRL_COUNTFLAG) == 0){} //if the 16th bit of the CTRL register isn't set, keep delaying
	}
	/*Disable systick*/
	SysTick->CTRL = 0; 
}