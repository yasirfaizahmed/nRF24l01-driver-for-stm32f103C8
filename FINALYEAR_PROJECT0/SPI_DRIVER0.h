#ifndef SPI_DRIVER0_H_INCLUDED
#define SPI_DRIVER0_H_INCLUDED

#include <stdint.h>

/************ function prototyping ***********/
void SPI_nrf_GPIO_setup(void);	//pretty self explainatory
void SPI_nrf_setup(void);		//SPI1 at 571Kbps, max 31Mbps
uint8_t SPI_nrf_read_reg(uint8_t addr);	//reads the nRF registers
uint8_t SPI_nrf_rx(uint8_t value);	//sends the read commanad, can return any register value
uint8_t SPI_nrf_read_status(void);	//returns hte contents of STATUS reg only
uint8_t SPI_nrf_write(uint8_t addr, uint8_t data);	//to write into the passed address register, return also the status register contents



#endif

