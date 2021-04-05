#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "nRFl01_0.h"
#include "GPIO_DRIVER2.h"
#include "TIM_DRIVER0.h"
#include "usart_debug0.h"		//only for debugging through UART serial to USB
#include "SPI_DRIVER0.h"



int main(){
	clock_setup();
	
	SPI_nrf_GPIO_setup();
	
	tim_setup();
	
	SPI_nrf_setup();
	
	usart1_setup();//for debugging
	
	

	
	
	while(1){
		for(int i=0;i<0x0A;i++) UART_TX(1, SPI_nrf_read_reg(R_REGISTER | i) );
		delay_ms(50);
		
	}
	
	
}


