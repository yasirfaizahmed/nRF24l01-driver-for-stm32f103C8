#include "../inc/TIM_DRIVER0.h"

/************************************** clock setup  ****************************************/
void clock_setup(){
	RCC->CR |= RCC_CR_HSION;	//HSI on
	while( !(RCC_CR_HSIRDY & (RCC->CR)) );	//wait till its ready
	
	//clocks for peripherals 
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//enable clock forport A
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//enable clock for alternate functions
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;	//enable clock for SPI1

}


/************************************ tim setup function ************************************/
void tim_setup(){
	//timer setup (TIM4)
	delay_tim_init();	//initilize the TIM4 needed for delays
	
}


/************************************ tim_init function ************************************/
void delay_tim_init(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC = 72;		//72MHz sys_clock / 72 = 1MHz
	TIM4->CR1 |= TIM_CR1_CEN;
	
}
 

/************************************ delay_us function ************************************/
void delay_us(int us){
	TIM4->ARR = us;
	while(1){
		if((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF){	//waiting for the flag to turn on if overflow occurs
			TIM4->SR &= ~(TIM_SR_UIF);	//clearing the flag
			break;
		}
	}
}


/************************************ delay_ms function ***************************************/
void delay_ms(int ms){
	for(int i=0;i<ms;i++) delay_us(1000);
}


/************************************ analog_write function ************************************/
void analog_write(IOP IOP_value, GPIO_TypeDef* port, TIM_TypeDef* TIM, int channel, int pin, int PWM_mode, bool parity_bit, float duty_cycle){
	
	if(TIM == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	if(TIM == TIM8) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	if(TIM == TIM9) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//TIM in APB2ENR
	if(TIM == TIM10) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	if(TIM == TIM11) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	if(TIM == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	if(TIM == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	//TIM in APB1ENR
	if(TIM == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//alternate function (in our case here PWM)
	pin_mode(IOP_value, port, pin, op_50MHz, op_afpp);	//alternate function
	
	TIM->PSC = 72;
	TIM->ARR = 1000;	//1kHz
	
	TIM->CCER |= ( 1<<4*(channel-1) );	// Capture/Compare output enable 
	if(channel == 1 || channel == 2){	//PWM modes for channel 1,2
		if(PWM_mode == 1) TIM->CCMR1 |= ( 6<<(channel*4 + ((channel-1)* 4)) );	//PWM mode 1
		else TIM->CCMR1 |= ( 7<<(channel*4 + ((channel-1)* 4)) );	//PWM_mode mode 2
	}
	else{	//PWM modes for channel 3,4
		if(PWM_mode == 1) TIM->CCMR2 |= ( 6<<((channel-2)*4 + ((channel-3)* 4)) );	//PWM mode 1
		else TIM->CCMR2 |= ( 7<<((channel-2)*4 + ((channel-3)* 4)) );	//PWM_mode mode 2
	}
	
	if(parity_bit == true) TIM->CCER |= ( 1<<(4*(channel-1) + 1) );	//parity options
	
	
	
	switch(channel){	//duty cycle
		case 1:TIM->CCR1 = (TIM->ARR) * duty_cycle;
		case 2:TIM->CCR2 = (TIM->ARR) * duty_cycle;
		case 3:TIM->CCR3 = (TIM->ARR) * duty_cycle;
		case 4:TIM->CCR4 = (TIM->ARR) * duty_cycle;
		
	}
	
	TIM->CR1 |= TIM_CR1_CEN;	//turn on the timer
}
