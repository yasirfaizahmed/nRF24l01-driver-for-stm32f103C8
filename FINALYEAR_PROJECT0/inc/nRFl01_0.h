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
#define R_REGISTER          (uint8_t) 0x00
#define W_REGISTER          (uint8_t) 0x20
#define R_RX_PAYLOAD        (uint8_t) 0x61
#define W_TX_PAYLOAD        (uint8_t) 0xA0
#define FLUSH_TX            (uint8_t) 0xE1
#define FLUSH_RX            (uint8_t) 0xE2
#define REUSE_TX_PL         (uint8_t) 0xE3
#define ACTIVATE            (uint8_t) 0x50
#define R_RX_PL_WID         (uint8_t) 0x60
#define W_ACK_PAYLOAD       (uint8_t) 0xA8
#define W_TX_PAYLOAD_NOACK  (uint8_t) 0xB0
#define NOP                 (uint8_t) 0xFF



/*********************** nRF Bit definitions ********************/
/******** CONFIG *******/
#define MASK_RX_DR        (uint8_t) 0x06
#define MASK_TX_DS        (uint8_t) 0x05
#define MASK_MAX_RT       (uint8_t) 0x04
#define EN_CRC            (uint8_t) 0x08
#define EN_CRC_MASK				(uint8_t) 0xF7
#define CRCO              (uint8_t) 0x04
#define PWR_UP            (uint8_t) 0x02
#define PWR_DN						(uint8_t) 0x00//
#define PWR_MASK					(uint8_t) 0xFD//
#define PRIM_RX           (uint8_t) 0x01
#define PRIM_TX						(uint8_t) 0x00//
#define PRIM_MASK					(uint8_t) 0xFE//

/******** EN_AA *******/
#define ENAA_P5           (uint8_t) 0x05
#define ENAA_P4           (uint8_t) 0x04
#define ENAA_P3           (uint8_t) 0x03
#define ENAA_P2           (uint8_t) 0x02
#define ENAA_P1           (uint8_t) 0x01
#define ENAA_P0           (uint8_t) 0x00

/****** EN_RXADDR ****/
#define ERX_P5            (uint8_t) 0x05
#define ERX_P4            (uint8_t) 0x04
#define ERX_P3            (uint8_t) 0x03
#define ERX_P2            (uint8_t) 0x02
#define ERX_P1            (uint8_t) 0x01
#define ERX_P0            (uint8_t) 0x00

/****** SETUP_AW *****/
#define AW_3B             (uint8_t) 0x01
#define AW_4B             (uint8_t) 0x02
#define AW_5B             (uint8_t) 0x03

/**** SETUP_RETR ****/
#define RETR_ARC_0        (uint8_t) 0x01
#define RETR_ARC_1        (uint8_t) 0x02
#define RETR_ARC_2      	(uint8_t) 0x04
#define RETR_ARC_3      	(uint8_t) 0x08
#define RETR_ARC_DIS      (uint8_t) 0x00//
#define RETR_ARC_MASK			(uint8_t) 0xF0//
#define RETR_ARD_0      	(uint8_t) 0x10
#define RETR_ARD_1      	(uint8_t) 0x20
#define RETR_ARD_2      	(uint8_t) 0x40
#define RETR_ARD_3      	(uint8_t) 0x80
#define RETR_ARD_DIS			(uint8_t) 0x00//
#define RETR_ARD_MASK			(uint8_t) 0x0F//

/******* RF_CH ******/
#define RF_CH_0           (uint8_t) 0x01
#define RF_CH_1           (uint8_t) 0x02
#define RF_CH_2           (uint8_t) 0x04
#define RF_CH_3           (uint8_t) 0x08
#define RF_CH_4           (uint8_t) 0x10
#define RF_CH_5           (uint8_t) 0x20
#define RF_CH_6           (uint8_t) 0x40

/****** RF_SETUP ****/
#define RF_PWR_0          (uint8_t) 0x02
#define RF_PWR_1          (uint8_t) 0x04
#define RF_PWR_MIN				(uint8_t) 0x00//
#define RF_PWR_MASK				(uint8_t) 0xF9//
#define RF_DR_250Kbps			(uint8_t) 0x20
#define RF_DR_MASK 				(uint8_t) 0xD7//
#define CONT_WAVE         (uint8_t) 0x80

/***** STATUS ******/
#define TX_FULL           (uint8_t) 0
#define RX_P_NO           (uint8_t) 1
#define MAX_RT            (uint8_t) 0x10
#define MAX_RT_MASK				(uint8_t) 0xEF//
#define TX_DS             (uint8_t) 0x20
#define TX_DS_MASK				(uint8_t) 0xDF//
#define RX_DR             (uint8_t) 0x40
#define RX_DR_MASK				(uint8_t) 0xBF//
#define STATUS_FLAG_CLEAR (uint8_t) 0x70//
#define STATUS_FLAGS_MASK (uint8_t) 0x8F//

/**** OBSERVE *****/
#define PLOS_CNT          (uint8_t) 4
#define ARC_CNT           (uint8_t) 0

/***** CD ********/
#define CD_BIT            (uint8_t) 0

/******* FIFO ****/
#define RX_EMPTY          (uint8_t) 0
#define RX_FULL           (uint8_t) 1
#define TX_EMPTY          (uint8_t) 4
#define FIFO_FULL         (uint8_t) 5
#define TX_REUSE          (uint8_t) 6

/********* DYNPD ****/
#define DPL_P0            (uint8_t) 0x01
#define DPL_P1            (uint8_t) 0x02
#define DPL_P2            (uint8_t) 0x04
#define DPL_P3            (uint8_t) 0x08
#define DPL_P4            (uint8_t) 0x10
#define DPL_P5            (uint8_t) 0x20
#define DPL_MASK					(uint8_t) 0xC0//

/***** FEATURE *****/
#define EN_DYN_ACK        (uint8_t) 0x01
#define EN_ACK_PAY        (uint8_t) 0x02
#define EN_DPL            (uint8_t) 0x04
#define EN_DPL_MASK				(uint8_t) 0xFB//



/********************* custom defines ********************/
/****** pipe number *******/
#define PIPE_0	0x00 
#define PIPE_1	0x01
#define PIPE_2	0x02
#define PIPE_3	0x03
#define PIPE_4	0x04
#define PIPE_5	0x05


/****** Offset addresses *****/
#define RX_ADDR_OFFSET  0x0A
#define RX_PW_OFFSET    0x11

/****** others *****/
#define CLEAR 0x00
#define DUMMY_BYTE 0x69


/********************* function prototyping **************/
void nrf_init(void);	//it sets the nRF in working condition
bool nrf_set_TX_ADDR(uint64_t tx_addr, int addr_width);	//sets the TX_ADDR	(need to work on it)
void nrf_ptx_init(void);	//initializes the PTX
uint8_t nrf_tx(uint8_t payload[]);	//transmit function
void nrf_prx_init(void);	//initializes the PRX
void nrf_rx(char payload[]);	//recieve the payload byte by byte 
uint8_t nrf_check_msg(void);	//returns 1 if data is ready in RX FIFO else returns 0

#endif

