#ifndef USART_DEBUG0_INCLUDED_H
#define USART_DEBUG0_INCLUDED_H


#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>
#include "stdlib.h"
#include "stdarg.h"



static void printMsg(char *msg, ...);
void usart1_setup(void);


void usart1_setup(){
	//Enabeling HSI, Not really necessary.
	RCC->CR |= RCC_CR_HSION;
	while( !(RCC->CR & RCC_CR_HSIRDY) );
	
	//Enabeling clock for APB2 for UART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//Enabeling clock for APB2 for GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//GPIO config PA10, PA9
	GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1;
	GPIOA->CRH &= ~(GPIO_CRH_CNF9_0);	//clearing the default bit
	
	//USART setup
	USART1->BRR = 0x1d4c;
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_UE;
	
}	
	

static void printMsg(char *msg, ...){
	
	char buff[80];
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);
	
	for(int i=0;i<strlen(buff);i++){
		USART1->DR = buff[i];
		while( !(USART1->SR & USART_SR_TXE) );
	}
}


#endif
