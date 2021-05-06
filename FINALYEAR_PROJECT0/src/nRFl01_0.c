#include "../inc/nRFl01_0.h"
#include <stm32f10x.h>
#include "../inc/GPIO_DRIVER2.h"
#include "../inc/TIM_DRIVER0.h"
#include "../inc/SPI_nrf_DRIVER0.h"

/*
void nRF_setup(){
	// SETUP REG (0x20) (0x0A)
	//command |= W_REGISTER | CONFIG;	//Or-ing the command with offset
	data |= PWR_UP;	//power up the nRF
	data |= EN_CRC;	//enabeling CRC
	SPI_write_uint8_t(CONFIG, data);
	data = CLEAR;	//clearing data buffer
	delay_us(1000);
	
	//SETUP_AW REG	(0x23) (0x03)
	data |= AW_5B;	//5Bytes of address width
	SPI_write_uint8_t(SETUP_AW, data);
	data = CLEAR;
	delay_us(1000);
	
	//SETUP_RETR REG	(0x24) (0xFF)
	data |= RETR_ARC_0 | RETR_ARC_1 | RETR_ARC_2 | RETR_ARC_3;	//15 re_transmit retries 
	data |= RETR_ARD_0 | RETR_ARD_1 | RETR_ARD_2 | RETR_ARD_3;	//wait for 4000us  
	SPI_write_uint8_t(SETUP_RETR, data);
	data = CLEAR;
	delay_us(1000);
	
	//STATUS REG	(0x27) (0x70)
	data |= 0x70;	//clearing flags 
	SPI_write_uint8_t(STATUS, data);
	data = CLEAR;	
	delay_us(1000);
}
*/

void nrf_init(){
	pin_mode(IOPA, GPIOA, 3, op_50MHz, op_gppp);	//CE pin setup
	digital_writepin(GPIOA, 3, LOW);	//A HIGH to LOW transition is what causes the tranmission to start from nTF PTX
	
	SPI_nrf_write_bit(CONFIG, PWR_DN, PWR_MASK);
	delay_ms(15);	//to get the nRF in a stable state
	//SPI_nrf_write_bit(CONFIG, PWR_UP, PWR_MASK);	//turns-on the nRF
	//SPI_nrf_write_bit(CONFIG, CRCO);	//2byte CRC scheme
	SPI_nrf_write_bit(CONFIG, EN_CRC, EN_CRC_MASK);	//enabeling auto ACK
	//SPI_nrf_write_bits(SETUP_AW, AW_3B);	//setting Address width to 3Bytes
	
	SPI_nrf_write_bit(RF_SETUP, RF_DR_250Kbps, RF_DR_MASK);	//setting up the lowest data speeds
	SPI_nrf_write_bits(RF_SETUP, (RF_PWR_0|RF_PWR_1), RF_PWR_MASK);	//setting the highest power 
	
	SPI_nrf_write_bits(SETUP_RETR, (RETR_ARD_0|RETR_ARD_1|RETR_ARD_2|RETR_ARD_3), RETR_ARD_MASK);	//waiting 4000uS for onother Auto Retransmission
	SPI_nrf_write_bits(SETUP_RETR, (RETR_ARC_0|RETR_ARC_1|RETR_ARC_2), RETR_ARC_MASK);	//7 re_transmit on fail of AA
	
	SPI_nrf_write_bit(FEATURE, EN_DPL, EN_DPL_MASK);	//enabling dynamic payload
	SPI_nrf_write_bits(DYNPD, DPL_P0, DPL_MASK);	//enabelin in DPL reg also
	
	
}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// not working ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool nrf_set_TX_ADDR(uint64_t tx_addr, int addr_width){	//sets the TX_ADDR, returns 1 if successfull else returns 0
	uint8_t data;
	uint8_t mask = 0xFF, temp = tx_addr;
	uint8_t i = 0;
	
	digital_writepin(GPIOA, 4, LOW);
	
	SPI_nrf_rx_tx(W_REGISTER | TX_ADDR);	//sending W_REGISTER into TX_ADDR
	while(i < addr_width){
		temp = temp & (mask<<(i*8));
		data = temp>>(8*i);
		SPI_nrf_rx_tx(data);
		temp = tx_addr;
		i++;
	}
	
	digital_writepin(GPIOA, 4, HIGH);
}
*/



void nrf_ptx_init(void){
	SPI_nrf_write_bits(STATUS, STATUS_FLAG_CLEAR, STATUS_FLAGS_MASK);	//clearing flags in STATUS reg

	SPI_nrf_cmd(FLUSH_RX);	//flushing RX first then TX FIFO
	SPI_nrf_cmd(FLUSH_TX);	//cleaning the TX FIFO making it ready to receive fresh payload
	
	SPI_nrf_write_bit(CONFIG, PRIM_TX, PRIM_MASK);	//as PTX now
	SPI_nrf_write_bit(CONFIG, PWR_UP, PWR_MASK);	//turns-on the nRF
	delay_ms(15);	//some delay till stability is achieved
	
	digital_writepin(GPIOA, 3, HIGH);	//setting CE pin HIGH (ready to transmit)
	  
}

uint8_t nrf_tx(uint8_t payload[]){	//transmitts the uint8_t array byte by byte, if sucsessfull returns1, else returns 0
	
	pin_mode(IOPC, GPIOC, 13, op_50MHz, op_gppp);	//for debugging
	digital_writepin(GPIOC, 13, LOW);

	int i = 0;
	if( SPI_nrf_read_reg(CONFIG) & (PRIM_RX) ) nrf_ptx_init();	//if in PRX mode, goto nrf_ptx_init
	if( SPI_nrf_read_reg(DYNPD) & (DPL_P0) ){	//if dynamic payload is turned off 
		SPI_nrf_write_bit(FEATURE, EN_DPL, EN_DPL_MASK);	//enabling dynamic payload
		SPI_nrf_write_bits(DYNPD, DPL_P0, DPL_MASK);	//enabelin in DPL reg also
	}
	
	digital_writepin(GPIOA, 4, LOW);	//making CS LOW 
	SPI_nrf_rx_tx(W_TX_PAYLOAD);		//sending the W_TX_PAYLOAD command
	while(payload[i] != '\0'){	//sending byte by byte 
		SPI_nrf_rx_tx(payload[i]);
		i++;
	}
	digital_writepin(GPIOA, 4, HIGH);	//making CS HIGH
	
	if( (SPI_nrf_read_status()) & (TX_DS) ){	//transmitted and recieved the ACK also
		//trigger = 0;
		//msg_status = 0;
		SPI_nrf_write_bit(STATUS, TX_DS, TX_DS_MASK);	//resettign the flag
		digital_writepin(GPIOC, 13, HIGH);	//indication
		delay_ms(100);
		digital_writepin(GPIOC, 13, LOW);
		return 1;
	}
				
	if( (SPI_nrf_read_status()) & (MAX_RT) ){	//MAX_RT flag reached
		SPI_nrf_write_bit(STATUS, MAX_RT, MAX_RT_MASK);	//clearing the flag by writing 1 into it
		digital_writepin(GPIOC, 13, HIGH);	//indication
		delay_ms(1000);
		digital_writepin(GPIOC, 13, LOW);
		delay_ms(5000);
		return 0;
	}
				
	else if( (SPI_nrf_read_status()) & (MAX_RT) ){	//TX_FULL flag reached
		//msg_status = 0;
		//trigger = 0;
		SPI_nrf_cmd(FLUSH_TX);	//flushing the FIFO
		digital_writepin(GPIOC, 13, HIGH);	//indication
		delay_ms(5000);
		digital_writepin(GPIOC, 13, LOW);
		delay_ms(5000);
		return 0;
	}
					
	
	
}

void nrf_prx_init(void){	//initializes the PRX
	SPI_nrf_write_bits(STATUS, STATUS_FLAG_CLEAR, STATUS_FLAGS_MASK);	//clearing flags in STATUS reg
	
	SPI_nrf_cmd(FLUSH_RX);	//flushing RX first then TX FIFO
	SPI_nrf_cmd(FLUSH_TX);	//cleaning the TX FIFO making it ready to receive fresh payload
	
	SPI_nrf_write_bit(CONFIG, PRIM_RX, PRIM_MASK);	//as PRX now
	SPI_nrf_write_bit(CONFIG, PWR_UP, PWR_MASK);	//turns-on the nRF
	delay_ms(15);	//some delay till stability is achieved
	
	digital_writepin(GPIOA, 3, HIGH);	//setting CE pin HIGH (ready to recieve)
	
} 

void nrf_rx(char payload[]){	//recieve the payload byte by byte 
	uint8_t payload_width = 0;
	
	payload_width = SPI_nrf_cmd(R_RX_PL_WID);	//send the command to get the payload width
	digital_writepin(GPIOA, 4, LOW);	//CS LOW
	SPI_nrf_rx_tx(R_RX_PAYLOAD);	//recieve command
	for(uint8_t i=0;i<payload_width;i++){
		payload[i] = SPI_nrf_rx_tx(R_RX_PAYLOAD);	//sendign dummpybyte 
	}
	digital_writepin(GPIOA, 4, HIGH);	//CS HIGH
	SPI_nrf_write_bit(STATUS, RX_DR, RX_DR_MASK);	//clearing the RX_DR falg
	SPI_nrf_cmd(FLUSH_RX);	//clear RX FIFO
	
}

uint8_t nrf_check_msg(void){	//returns 1 if data is ready in RX FIFO else returns 0
	uint8_t status = SPI_nrf_read_reg(STATUS);
	if( (SPI_nrf_read_status()) & (RX_DR) ) return 1;	//Data ready RX FIFO
	else return 0;
}
