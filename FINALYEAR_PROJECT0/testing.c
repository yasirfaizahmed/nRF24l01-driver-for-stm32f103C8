#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "inc/nRFl01_0.h"
#include "inc/GPIO_DRIVER2.h"
#include "inc/TIM_DRIVER0.h"
#include "inc/usart_debug0.h"		//only for debugging through UART serial to USB
#include "inc/SPI_nrf_DRIVER0.h"



int main(){
	clock_setup();
	
	SPI_nrf_GPIO_setup();
	
	tim_setup();
	
	SPI_nrf_setup();
	
	usart1_setup();
	
	//testing linrary functions here starts
	
	nrf_setup();
	nrf_set_TX_ADDR(0x1234567890, 5);
	while(1){
		UART_TX(1, SPI_nrf_read_reg(TX_ADDR));
		delay_ms(2000);
	}

}


