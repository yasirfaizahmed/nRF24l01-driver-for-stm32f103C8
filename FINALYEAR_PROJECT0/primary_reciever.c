#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "inc/nRFl01_0.h"
#include "inc/GPIO_DRIVER2.h"
#include "inc/TIM_DRIVER0.h"
#include "inc/usart_debug0.h"	//for debugging only
#include "inc/SPI_nrf_DRIVER0.h"


char payload[32];	//our paylaod

int main(){
	clock_setup();	//setting up the clock for the used peripherals
	
	SPI_nrf_GPIO_setup(); //setting up the GPIO pins required for SPI1
	
	tim_setup();	//setting up the timers mainly for delays
	
	SPI_nrf_setup();	//setting up the SPI to communicate to nRF
	
	usart1_setup();	//setting up the uart for debugging and for receiving the payload
	
	nrf_init();	//finally setting up the nRF 
	
	nrf_prx_init();	//and now the initilize the primary reciever
	
	
	
	uint8_t payload_width = 0;
	while(1){
		
		//for(int i=0;i<0x1E;i++) UART_TX(1, SPI_nrf_read_reg(i));
		//delay_ms(1000);
		if(nrf_check_msg()){	//check if the message has been recieved or no
			nrf_rx(payload);	//if yes, then coppy all the FIFO to the payload array
			payload_width = SPI_nrf_cmd(R_RX_PL_WID);	//getting the payload width 
			
			UART_SEND(1,payload, payload_width);	//sending the payyload array through uart 1
			
			
			
			
		}
		
		
	}
	
	
	
	
}

