#ifndef NRFL01_0_H_INCLUDED
#define NRFL01_0_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

/*********************** nRF Register offset addresses ********************/
#define CONFIG				(uint8_t) 0x00
#define EN_AA 				(uint8_t) 0x01
#define EN_RXADDR 		(uint8_t) 0x02
#define SETUP_AW 			(uint8_t) 0x03
#define SETUP_RETR 		(uint8_t) 0x04
#define RF_CH 				(uint8_t) 0x05
#define RF_SETUP 			(uint8_t) 0x06
#define STATUS				(uint8_t) 0x07
#define OBSERVE_TX		(uint8_t) 0x08
#define CD						(uint8_t) 0x09

#define RX_ADDR_P0		(uint8_t) 0x0A
#define RX_ADDR_P1		(uint8_t) 0x0B
#define RX_ADDR_P2		(uint8_t) 0x0C
#define RX_ADDR_P3		(uint8_t) 0x0D
#define RX_ADDR_P4		(uint8_t) 0x0E
#define RX_ADDR_P5		(uint8_t) 0x0F

#define TX_ADDR				(uint8_t) 0x10

#define RX_PW_P0			(uint8_t) 0x11
#define RX_PW_P1			(uint8_t) 0x12
#define RX_PW_P2			(uint8_t) 0x13
#define RX_PW_P3			(uint8_t) 0x14
#define RX_PW_P4			(uint8_t) 0x15
#define RX_PW_P5			(uint8_t) 0x16

#define FIFO_STATUS		(uint8_t) 0x10
#define DYNPD 				(uint8_t) 0x1C
#define FEATURE				(uint8_t) 0x1D


/*********************** nRF Register commands ********************/
#define R_REGISTER 					(uint8_t) 0x00
#define W_REGISTER					(uint8_t) 0x01
#define R_RX_PAYLOAD				(uint8_t) 0x61
#define W_TX_REGISTER 			(uint8_t) 0xA0
#define FLUSH_TX						(uint8_t) 0xE1
#define FLUSH_RX 						(uint8_t) 0xE2
#define REUSE_TX_PL 				(uint8_t) 0xE3
#define ACTIVATE		 				(uint8_t) 0x50
#define R_RX_PL_WID					(uint8_t) 0x60
#define W_ACK_PAYLOAD				(uint8_t) 0xA8
#define W_TX_PAYLOAD_NOACK	(uint8_t) 0xB0
#define NOP									(uint8_t) 0xFF

#endif