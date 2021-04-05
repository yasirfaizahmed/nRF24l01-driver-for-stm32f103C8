#ifndef SPI_DRIVER0_H_INCLUDED
#define SPI_DRIVER0_H_INCLUDED

/************ function prototyping ***********/
void SPI_nrf_GPIO_setup(void);	//pretty self explainatory
void SPI_nrf_setup(void);		//SPI1 at 571Kbps, max 31Mbps
char SPI_nrf_read_reg(char addr);	//reads the nRF registers
char SPI_nrf_rx(char value);	//sends the read commanad, returns the register value
char SPI_nrf_read_status(void);	//returns hte contents of STATUS reg only



#endif

