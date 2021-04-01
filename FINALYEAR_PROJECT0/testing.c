#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "nRFl01_0.h"
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"
#include "usart_debug0.h"		//only for debugging through UART serial to USB




uint8_t command = 0x00;	//command to be sent to nRF
uint8_t data = 0x00;	//data which will be sent to nRF


int main(){
	clock_setup();
	
	GPIO_setup();
	
	tim_setup();
	
	SPI_setup();
	
	while(1){
		nRF_setup();
		delay_ms(50);
		
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
	
	//digital_writepin(GPIOA, 4, HIGH);
	GPIOA->BSRR |= (1<<4);	
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

	
void SPI_send_uint8_t(uint8_t data){
	//very standard SPI TX protocol
	delay_us(20);
	GPIOA->BSRR |= (1<<4);	
	//digital_writepin(GPIOA, 4, LOW);
	SPI1->DR = data;
	while( !((SPI1->SR) & SPI_SR_BSY) );
	//digital_writepin(GPIOA, 4, HIGH);
	GPIOA->BRR |= (1<<4);	
	
}


void nRF_setup(){
	// SETUP REG (0x20) (0x0A)
	command |= W_REGISTER | CONFIG;	//Or-ing the command with offset
	SPI_send_uint8_t(command);
	command = CLEAR;	//clearing command buffer
	delay_ms(5);
	data |= PWR_UP;	//power up the nRF
	//data |= PRIM_RX;	//as a Primary TX device
	//data |= CRCO;	//CRC encoding scheme, 2bytes 
	data |= EN_CRC;	//enabeling CRC
	SPI_send_uint8_t(data);
	data = CLEAR;	//clearing data buffer
	delay_ms(5);
	
	//SETUP_AW REG	(0x23) (0x03)
	command |= W_REGISTER | SETUP_AW;	// Or-ring with offset 
	SPI_send_uint8_t(command);
	command = CLEAR;
	delay_ms(5);
	data |= AW_5B;	//5Bytes of address width
	SPI_send_uint8_t(data);
	data = CLEAR;
	delay_ms(5);
	
	//SETUP_RETR REG	(0x24) (0xFF)
	command |= W_REGISTER | SETUP_RETR;	//Or-ring with offset
	SPI_send_uint8_t(command);
	command = CLEAR;
	delay_ms(5);
	data |= RETR_ARC_0 | RETR_ARC_1 | RETR_ARC_2 | RETR_ARC_3;	//15 re_transmit retries 
	data |= RETR_ARD_0 | RETR_ARD_1 | RETR_ARD_2 | RETR_ARD_3;	//wait for 4000us  
	SPI_send_uint8_t(data);
	data = CLEAR;
	delay_ms(5);
	
	//STATUS REG	(0x27) (0x00)
	command |= W_REGISTER | STATUS;	//Or-ing with offset
	SPI_send_uint8_t(command);
	command = CLEAR;
	delay_ms(5);
	data = CLEAR;	//clearing all the flags 
	SPI_send_uint8_t(data);
	delay_ms(5);
	
}
