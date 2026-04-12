#include "adxl345.h"
#include "spi.h"

void adxl_init(void){
	//enable spi GPIO
	spi_gpio_init(); 
	spi1_config(); 
	adxl_write(ADXL345_REG_DATA_FORMAT, ADXL345_RANGE_4G); 
	adxl_write(ADXL345_REG_POWER_CTL, ADXL345_RESET); 
	adxl_write(ADXL345_REG_POWER_CTL, ADXL345_MEASURE_BIT); 
}

void adxl_read(uint8_t address, uint8_t *rxdata){
	//set read op
	address |= ADXL345_READ_OPERATION;
	//enable multi-byte 
	address |= ADXL345_MULTI_BYTE_ENABLE; //enable multi-byte
	cs_enable(); //pull cs line low to enable slave
	spi1_transmit(&address,1); //send address
	spi1_receive(rxdata,6); //read 6 bytes
	cs_disable(); //pull cs line high to disable slave
}

void adxl_write(uint8_t address, uint8_t value){
	uint8_t data[2]; 
	//enable multi-byte place address into buffer
	data[0] = address | ADXL345_MULTI_BYTE_ENABLE;
	//put data into buffer
	data[1] = value; 
	cs_enable(); 
	spi1_transmit(data, 2); 
	cs_disable(); 
}

