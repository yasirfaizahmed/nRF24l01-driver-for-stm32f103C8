#ifndef SPI_NRF_DRIVER0_H_INCLUDED
#define SPI_NRF_DRIVER0_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

/************ function prototyping ***********/
void SPI_nrf_GPIO_setup(void);	//pretty self explainatory
void SPI_nrf_setup(void);		//SPI1 at 571Kbps, max 31Mbps
uint8_t SPI_nrf_read_reg(uint8_t addr);	//reads the nRF registers
uint8_t SPI_nrf_rx_tx(uint8_t value);	//sends the value, can return any status register value
uint8_t SPI_nrf_read_status(void);	//returns hte contents of STATUS reg only
bool SPI_nrf_write_bit(uint8_t addr, uint8_t bit, uint8_t mask);	//to write into the passed address register, returns 1 if successfully written else returns 0
bool SPI_nrf_write_bits(uint8_t addr, uint8_t bits, uint8_t mask);	//does the same thing as above, but instead writes multiple bits at once in the reg


#endif

