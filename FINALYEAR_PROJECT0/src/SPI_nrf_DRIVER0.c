#include "../inc/SPI_nrf_DRIVER0.h"
#include <stm32f10x.h>
#include "../inc/GPIO_DRIVER2.h"
#include "../inc/nRFl01_0.h"
#include "../inc/TIM_DRIVER0.h"

void SPI_nrf_setup(){
	//SPI setup
	SPI1->CR1 |= SPI_CR1_MSTR;	//master mode
	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2;	//at 281Kbps, max 31Mbps
	//SPI1->CR1 |= SPI_CR1_SSI;	//Software slave management enabled
	SPI1->CR2 |= SPI_CR2_SSOE;	//SS o/p enable
	SPI1->CR1 |= SPI_CR1_SPE;	//turn on the SPI
}


void SPI_nrf_GPIO_setup(){
	//GPIO pin setup as alternate function
	pin_mode(IOPA, GPIOA, 7, op_50MHz, op_afpp);	//MOSI pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 6, ip, ip_pupd);	//MISO pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 5, op_50MHz, op_afpp);	//SCK pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 4, op_50MHz, op_gppp);	//CS pin as GPIO general_puspose_pin can run upto 50MHz
	
	digital_writepin(GPIOA, 4, HIGH);	//setting CS HIGH
}



uint8_t SPI_nrf_rx_tx(uint8_t value){
	uint8_t contents;
	SPI1->DR = value;
	while( (SPI1->SR) & (SPI_SR_BSY) );
	
	while( (SPI1->SR) & (SPI_SR_RXNE) ) contents = SPI1->DR;
	
	return contents;
}


uint8_t SPI_nrf_read_reg(uint8_t addr){
	uint8_t reg_contents;
	digital_writepin(GPIOA, 4, LOW);	//bring the CS lOW

	SPI_nrf_rx_tx(R_REGISTER | addr);	//sending R_REGISTER command
	reg_contents = SPI_nrf_rx_tx(R_REGISTER | addr);	//sending it again and in return taking nRF register data from MISO 

	digital_writepin(GPIOA, 4, HIGH);	//bring the CS HIGH
	return reg_contents;
}


uint8_t SPI_nrf_read_status(void){
	uint8_t status_contents;
	digital_writepin(GPIOA, 4, LOW);
	
	status_contents = SPI_nrf_rx_tx(R_REGISTER | STATUS);
	return status_contents;
}


bool SPI_nrf_write_bit(uint8_t addr, uint8_t bit, uint8_t mask){//to write into the passed address register, returns 1 if successfully written else returns 0
	uint8_t reg_content, new_content;	//reg_contents are the present data in reg we are about to write
	reg_content = SPI_nrf_read_reg(addr);	//taking the reg contents before over-writing
	new_content = (mask & reg_content) | bit;	//erasing the aboutToBeModified bit from the presnt reg_content and Oring it with new one
	digital_writepin(GPIOA, 4, LOW);
	
	SPI_nrf_rx_tx(W_REGISTER | addr);	//sending W_REGISTER command
	SPI_nrf_rx_tx(new_content);	//writing new contents
	
	digital_writepin(GPIOA, 4, HIGH);
	
	reg_content = SPI_nrf_read_reg(addr);	//taking the new contents back to verify
	if(reg_content == new_content) return 1;
	else return 0;
}


bool SPI_nrf_write_bits(uint8_t addr, uint8_t bits, uint8_t mask){	//does the same thing as above, but instead writes multiple bits at once in the reg
	uint8_t reg_content, new_content;
	reg_content = SPI_nrf_read_reg(addr);
	new_content = (mask & reg_content) | bits;
	digital_writepin(GPIOA, 4, LOW);
	
	SPI_nrf_rx_tx(W_REGISTER|addr);
	SPI_nrf_rx_tx(new_content);
	
	digital_writepin(GPIOA, 4, HIGH);
	
	reg_content = SPI_nrf_read_reg(addr);
	if(reg_content == new_content) return 1;
	else return 0;
	
}

