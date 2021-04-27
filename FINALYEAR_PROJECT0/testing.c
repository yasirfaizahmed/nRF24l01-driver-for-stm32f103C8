#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "inc/nRFl01_0.h"
#include "inc/GPIO_DRIVER2.h"
#include "inc/TIM_DRIVER0.h"
#include "inc/usart_debug0.h"		//only for debugging through UART serial to USB
#include "inc/SPI_nrf_DRIVER0.h"

unsigned short trigger = 1, msg_status = 1, nrf_irq = 0;

uint8_t payload[32] = "asdf";
state STATE = HIGH;
int main(){
	clock_setup();
	
	SPI_nrf_GPIO_setup();
	
	tim_setup();
	
	SPI_nrf_setup();
	
	usart1_setup();
	
	//testing linrary functions here starts
	
	nrf_init();
	nrf_ptx_init();
	for(int i=0;i<=0x1D;i++) UART_TX(1, SPI_nrf_read_reg(i));
	
	pin_mode(IOPC, GPIOC, 13, op_50MHz, op_gppp);	//for debugging
	digital_writepin(GPIOC, 13, LOW);
	
	pin_mode(IOPA, GPIOA, 0, op_50MHz, ip_pupd);	//interrupt IEQ pin
	__disable_irq();
	AFIO->EXTICR[0] |= 0x00;	//PA0 as interrupt input
	EXTI->IMR |= 1;
	EXTI->FTSR |=1;
	NVIC_EnableIRQ(EXTI0_IRQn);
	__enable_irq();
	
	
	while(1){
		
		
		
		
		if(trigger){
			nrf_tx(payload);	//transmit char by char
			
			msg_status = 1;	//after transmitting payload, we need to see what happened to the transmitted payload
			while(msg_status){
					
				
				if( (SPI_nrf_read_status()) & (TX_DS) ){	//transmitted and recieved the ACK also
					//trigger = 0;
					//msg_status = 0;
					SPI_nrf_write_bit(STATUS, TX_DS, TX_DS_MASK);	//resettign the flag
					digital_writepin(GPIOC, 13, HIGH);	//indication
				}
				
				if( (SPI_nrf_read_status()) & (MAX_RT) ){	//MAX_RT flag reached
					SPI_nrf_write_bit(STATUS, MAX_RT, MAX_RT_MASK);	//resettign the flag
					digital_writepin(GPIOC, 13, HIGH);
					delay_ms(5000);
					digital_writepin(GPIOC, 13, LOW);
					delay_ms(1000);
				}
				
				else if( (SPI_nrf_read_status()) & (MAX_RT) ){	//TX_FULL flag reached
					//msg_status = 0;
					//trigger = 0;
					SPI_nrf_rx_tx(FLUSH_TX);
					digital_writepin(GPIOC, 13, HIGH);
					delay_ms(10000);
					digital_writepin(GPIOC, 13, LOW);
					delay_ms(2000);
				}
				
				
			}
			
			
		}
		
		
		
		
		
		
		
	}



}

void EXTI0_IRQHandler()
{
	EXTI->PR |= 1;
	trigger = 1;
	STATE = !STATE;
	digital_writepin(GPIOC, 13, STATE);
}

