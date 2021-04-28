#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "inc/nRFl01_0.h"
#include "inc/GPIO_DRIVER2.h"
#include "inc/TIM_DRIVER0.h"
#include "inc/usart_debug0.h"		//only for debugging through UART serial to USB
#include "inc/SPI_nrf_DRIVER0.h"



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
	
	for(int i=0;i<=0x1D;i++) UART_TX(1, SPI_nrf_read_reg(i));	//just to get all the reg values once
	
	
	
	/*
	pin_mode(IOPA, GPIOA, 0, op_50MHz, ip_pupd);	//interrupt IEQ pin
	__disable_irq();
	AFIO->EXTICR[0] |= 0x00;	//PA0 as interrupt input
	EXTI->IMR |= 1;
	EXTI->FTSR |=1;
	NVIC_EnableIRQ(EXTI0_IRQn);
	__enable_irq();
	*/
	
	
	while(1){
		
		
		
		

			nrf_tx(payload);	//transmitt
			
			
			
			
		
		
		
		
		
		
		
	}



}
/*
void EXTI0_IRQHandler()
{
	EXTI->PR |= 1;
	trigger = 1;
	STATE = !STATE;
	digital_writepin(GPIOC, 13, STATE);
}
*/
