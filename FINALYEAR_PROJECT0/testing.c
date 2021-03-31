#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "nRFl01_0.h"
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"
#include "usart_debug0.h"


void clock_setup(void);
void GPIO_setup(void);
void tim_setup(void);
void SPI_setup(void);

int main(){
	
	
	
	
}

void clock_init(){
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
	SPI1->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_2;	//571Kbps (with 72MHz sys_clk)
	SPI1->CR2 |= SPI_CR2_SSOE;	//SS o/p enable
	SPI1->CR1 |= SPI_CR1_SPE;	//turn on the SPI
}
