#include "GPIO_DRIVER2.h"


/***************************************pin_mode function***********************************************/
void pin_mode(IOP IOP_value, GPIO_TypeDef* port, int pin, MODE mode_value, CNF cnf_value){
	
	RCC->APB2ENR |= IOP_value;
	
	uint32_t final_value = mode_value | (cnf_value<<2);	//getting the fianl value of the the modes combined
	
	if(pin <= 7){
		port->CRL &= ~( (uint32_t)(0xf<<(4*pin)) );	//clearing the default bits
		port->CRL |= ( final_value<<(4*pin) );	//writing in CRL 
	}
	
	if(pin > 7){
		port->CRH &= ~( (uint32_t)(0xf<<4*(pin-8)) );	//clearing the default bits
		port->CRH |= ( final_value<<(4*(pin-8)) );	//writing in CRH
	}
	
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////not working for pin number < 7 in port B//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************digital_write function ****************************************/
void digital_writepin(GPIO_TypeDef *port, int pin, state state_value){
	if(state_value == HIGH){
		port->ODR |= ( state_value<<pin );	//writing 1
	}
	else{
		port->ODR &=  ~(~state_value<<pin);	//writing 0
	}
	
}



/**********************************digital_lockpin function ****************************************/
void digital_lock(GPIO_TypeDef *port, int pin){
	if((port->LCKR & GPIO_LCKR_LCKK) == 1){	//if lock bit is set, reset it to remap the sequence 
		port->LCKR |= GPIO_LCKR_LCKK;
		port->LCKR &= ~(GPIO_LCKR_LCKK);	//To modify the bit, this is the sequence
		port->LCKR |= GPIO_LCKR_LCKK;
		
		port->LCKR &= ~(GPIO_LCKR_LCKK);	//finally clearing the LCKK bit
	}
	
	port->LCKR |= (1<<pin);
	
	port->LCKR |= GPIO_LCKR_LCKK;
	port->LCKR &= ~(GPIO_LCKR_LCKK);	//To modify the bit, this is the sequence
	port->LCKR |= GPIO_LCKR_LCKK;
	
	port->LCKR |= GPIO_LCKR_LCKK;	//finally locking the sequence
	
}


/**************************************digital_read function ******************************************/
state digital_readpin(GPIO_TypeDef *port, int pin){
	uint16_t mask = ( 1<<pin );
	if((port->IDR & mask) == mask){	//reading 1
		return HIGH;
	}
	else if((port->IDR & mask) == 0){	//reading 0
		return LOW;
	}
	return LOW;
}


	