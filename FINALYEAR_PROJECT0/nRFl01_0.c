#include "nRFl01_0.h"
#include <stm32f10x.h>
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"


/*

void nRF_setup(){
	// SETUP REG (0x20) (0x0A)
	//command |= W_REGISTER | CONFIG;	//Or-ing the command with offset
	data |= PWR_UP;	//power up the nRF
	data |= EN_CRC;	//enabeling CRC
	SPI_write_uint8_t(CONFIG, data);
	data = CLEAR;	//clearing data buffer
	delay_us(1000);
	
	//SETUP_AW REG	(0x23) (0x03)
	data |= AW_5B;	//5Bytes of address width
	SPI_write_uint8_t(SETUP_AW, data);
	data = CLEAR;
	delay_us(1000);
	
	//SETUP_RETR REG	(0x24) (0xFF)
	data |= RETR_ARC_0 | RETR_ARC_1 | RETR_ARC_2 | RETR_ARC_3;	//15 re_transmit retries 
	data |= RETR_ARD_0 | RETR_ARD_1 | RETR_ARD_2 | RETR_ARD_3;	//wait for 4000us  
	SPI_write_uint8_t(SETUP_RETR, data);
	data = CLEAR;
	delay_us(1000);
	
	//STATUS REG	(0x27) (0x70)
	data |= 0x70;	//clearing flags 
	SPI_write_uint8_t(STATUS, data);
	data = CLEAR;	
	delay_us(1000);
	
	
}


*/



