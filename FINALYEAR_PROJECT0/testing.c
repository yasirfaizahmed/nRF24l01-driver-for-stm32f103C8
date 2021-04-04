#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "nRFl01_0.h"
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"
#include "usart_debug0.h"		//only for debugging through UART serial to USB




uint8_t data = 0x00;	//data which will be sent to nRF


int main(){
	clock_setup();
	
	GPIO_setup();
	
	tim_setup();
	
	SPI_setup();
	
	usart1_setup();//for debugging
	
	
	for(uint8_t i=0x00;i<0x1D;i++){
			uint8_t val = SPI_read_uint8_t(i);
			//delay_ms(5);
			//printMsg("%d-%d\n", i, val);  
			USART1->DR = val;
			while( !(USART1->SR & USART_SR_TXE) );
			USART1->DR = '\n';
			while( !(USART1->SR & USART_SR_TXE) );
		}
	
	
	while(1){
		uint8_t val;
		for(int i=0;i<0xf;i++) val = SPI_read_uint8_t(i);
	}
	
}


void clock_setup(){
	RCC->CR |= RCC_CR_HSION;	//HSI on
	while( !(RCC_CR_HSIRDY & (RCC->CR)) );	//wait till its ready
	
	//clocks for peripherals 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//enable clock forport A
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//enable clock for alternate functions
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	//enable clock for SPI1

}


void GPIO_setup(){
	//GPIO pin setup as alternate function
	pin_mode(IOPA, GPIOA, 7, op_50MHz, op_afpp);	//MOSI pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 6, ip, ip_pupd);	//MISO pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 5, op_50MHz, op_afpp);	//SCK pin as GPIO alternate_pin can run upto 50MHz
	pin_mode(IOPA, GPIOA, 4, op_50MHz, op_gppp);	//CS pin as GPIO general_puspose_pin can run upto 50MHz
	

	digital_writepin(GPIOA, 4, HIGH);
}


void tim_setup(){
	//timer setup (TIM4)
	delay_tim_init();	//initilize the TIM4 needed for delays
	
}


void SPI_setup(){
	//SPI setup
	SPI1->CR1 |= SPI_CR1_MSTR;	//master mode
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2;	//at 571Kbps, max 31Mbps
	SPI1->CR1 |= SPI_CR1_SSI;	//Software slave management enabled
	SPI1->CR2 |= SPI_CR2_SSOE;	//SS o/p enable
	SPI1->CR1 |= SPI_CR1_SPE;	//turn on the SPI
}

	
void SPI_write_uint8_t(uint8_t addr, uint8_t data){
	//very standard SPI TX protocol
	delay_us(50);
	digital_writepin(GPIOA, 4, LOW);
	SPI1->DR = (W_REGISTER | addr);
	while(SPI1->SR & SPI_SR_BSY);
	delay_us(50);
	
	SPI1->DR = data;
	while(SPI1->SR & SPI_SR_BSY);
	digital_writepin(GPIOA, 4, HIGH);
	
}


uint8_t SPI_read_uint8_t(uint8_t addr){
	uint8_t reg_val;
	//sending the read command first along with address where we are reading from
	delay_us(50);
	digital_writepin(GPIOA, 4, LOW);
	SPI1->DR = (R_REGISTER | addr);
	while( (SPI1->SR) & (SPI_SR_BSY) );
	//start listening now
	delay_us(50);
	while( (SPI1->SR) & (0x01) ) reg_val = SPI1->DR;
	digital_writepin(GPIOA, 4, HIGH);
	
	return reg_val;
}





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




