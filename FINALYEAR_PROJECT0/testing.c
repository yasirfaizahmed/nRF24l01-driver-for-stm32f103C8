#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "nRFl01_0.h"
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"
#include "usart_debug0.h"		//only for debugging through UART serial to USB


void clock_setup(void);	//sets sys_clock at 72MHz using PLL and HSE, mostly done using the defaultly created startup file
void GPIO_setup(void);	//pretty self explainatory
void tim_setup(void);		//TIM4 for basic delays min 1us
void SPI_setup(void);		//SPI1 at 571Kbps, max 31Mbps

void nRF_setup(void);
void SPI_send_uint8_t(uint8_t);

uint8_t command = 0x00;	//command to be sent to nRF
uint8_t data = 0x00;	//data which will be sent to nRF


int main(){
	clock_setup();
	
	GPIO_setup();
	
	tim_setup();
	
	SPI_setup();
	
	while(1){
		nRF_setup();
		
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
	
}

void tim_setup(){
	//timer setup (TIM4)
	delay_tim_init();	//initilize the TIM4 needed for delays
	
}

void SPI_setup(){
	//SPI setup
	SPI1->CR1 |= SPI_CR1_MSTR;	//master mode
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2;	//at 571Kbps, max 31Mbps
	SPI1->CR2 |= SPI_CR2_SSOE;	//SS o/p enable
	SPI1->CR1 |= SPI_CR1_SPE;	//turn on the SPI
}

void nRF_setup(){
	command |= W_REGISTER | CONFIG;	//Or-ing the command with offset
	SPI_send_uint8_t(command);
	
	data |= PWR_UP;	//power up the nRF
	//command |= PRIM_RX;	//as a Primary TX device
	//command |= CRCO;	//CRC encoding scheme, 2bytes 
	data |= EN_CRC;	//enabeling CRC
	SPI_send_uint8_t(data);
	data = 0x00;	//clearing data buffer
	
	//digital_writepin(GPIOA, 4, LOW);	//bringing CE pin LOW
	
	
	
}
	
void SPI_send_uint8_t(uint8_t data){
	//very standard SPI TX protocol
	delay_us(20);
	digital_writepin(GPIOA, 4, LOW);
	SPI1->DR = data;
	while( !((SPI1->SR) & SPI_SR_BSY) );
	digital_writepin(GPIOA, 4, HIGH);
	
}
