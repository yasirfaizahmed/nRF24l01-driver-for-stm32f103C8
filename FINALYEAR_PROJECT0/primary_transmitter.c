#include <stdbool.h>
#include <stdint.h>
#include <stm32f10x.h>
#include "inc/nRFl01_0.h"
#include "inc/GPIO_DRIVER2.h"
#include "inc/TIM_DRIVER0.h"
#include "inc/usart_debug0.h"		//only for debugging through UART serial to USB
#include "inc/SPI_nrf_DRIVER0.h"



uint8_t payload[32] = "lmfao";	//our payload
int main(){
	clock_setup();	//setting up the clock for the used peripherals
	
	SPI_nrf_GPIO_setup(); //setting up the GPIO pins required for SPI1
	
	tim_setup();	//setting up the timers mainly for delays
	
	SPI_nrf_setup();	//setting up the SPI to communicate to nRF
	
	usart1_setup();	//setting up the uart for debugging and for receiving the payload
	
	nrf_init();	//finally setting up the nRF 
	
	nrf_ptx_init();	//and now the initilize the primary reciever
	
	//for(int i=0;i<=0x1D;i++) UART_TX(1, SPI_nrf_read_reg(i));	//just to get all the reg values once
	
	
	
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
		
		
		
		

		nrf_tx(payload);	//transmitting the payload
		
		//for(int i=0;i<=0x1D;i++) UART_TX(1, SPI_nrf_read_reg(i));	//just to get all the reg values once
			
		delay_ms(1000);
		
			
		
		
		
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
