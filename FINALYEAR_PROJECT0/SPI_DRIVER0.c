#include "SPI_DRIVER0.h"
#include <stm32f10x.h>
#include "GPIO_DRIVER2.h"
#include "nRFl01_0.h"


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



uint8_t SPI_nrf_rx(uint8_t value){
	uint8_t contents;
	SPI1->DR = value;
	while( (SPI1->SR) & (SPI_SR_BSY) );
	
	while( (SPI1->SR) & (SPI_SR_RXNE) ) contents = SPI1->DR;
	
	return contents;
}


uint8_t SPI_nrf_read_reg(uint8_t addr){
	uint8_t reg_contents;
	digital_writepin(GPIOA, 4, LOW);	//bring the CS lOW

	SPI_nrf_rx(R_REGISTER | addr);
	reg_contents = SPI_nrf_rx(R_REGISTER | addr);

	digital_writepin(GPIOA, 4, HIGH);	//bring the CS HIGH
	return reg_contents;
}


uint8_t SPI_nrf_read_status(void){
	uint8_t status_contents;
	digital_writepin(GPIOA, 4, LOW);
	
	status_contents = SPI_nrf_rx(R_REGISTER | STATUS);
	return status_contents;
}

uint8_t SPI_nrf_write(uint8_t addr, uint8_t data){
	uint8_t status_contents;
	digital_writepin(GPIOA, 4, LOW);
	SPI1->DR = (W_REGISTER | addr);
	while( (SPI1->SR) & (SPI_SR_BSY) );
	
	
	
	SPI1->DR = data;
	while( (SPI1->SR) & (SPI_SR_BSY) );
	
}


