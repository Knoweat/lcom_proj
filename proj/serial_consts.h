#ifndef _SERIAL_CONSTS_H
#define _SERIAL_CONSTS_H


#define BASE_FREQ 115200

#define COM1_BASE_ADDR 0x3F8
#define COM2_BASE_ADDR 0x2F8

#define COM1_IRQ 4
#define COM2_IRQ 3

#define UART_DELAY 10000

#define SER_EVT 7

#define RB_REG 0x00 /*> Receiver Buffer Register */
#define TH_REG 0x00 /*> Transmitter Holding Register */
#define DLL_REG 0x00 /*> Divisor Latch Least Significant Byte */
#define IE_REG 0x01 /*> Interrupt Enable Register */
#define DLM_REG 0x01 /*> Divisor Latch Most Significant Byte */
#define II_REG 0x02 /*> Interrupt Identification Register */
#define FC_REG 0x02 /*> FIFO Control Register */
#define LC_REG 0x03 /*> Line Control Register */
#define MC_REG 0x04 /*> Modem Control Register */
#define LS_REG 0x05 /*> Line Status Register */
#define MS_REG 0x06 /*> Modem Status Register */
#define S_REG 0x07

/*
 * @brief Line Control Register
 */
#define NR_BITS_CHAR 0x03
#define FIVE_BITS_CHAR 0x00
#define SIX_BITS_CHAR 0x01
#define SEVEN_BITS_CHAR 0x02
#define EIGHT_BITS_CHAR 0x03

#define NR_STOP_BITS 0x04
#define ONE_STOP_BIT 0x00
#define TWO_STOP_BITS 0x04

#define PARITY_CTRL 0x38
#define NO_PARITY 0x00
#define ODD_PARITY 0x08
#define EVEN_PARITY 0x18
#define PARITY_SET_ONE 0x28
#define PARITY_SET_ZERO 0x38

#define BREAK_CTRL 0x40
#define DLAB 0x80
#define ACCESS_DL 0x80
#define ACCESS_RD_WR 0x00

/*
 * @brief Line Status Register
 */
#define RCVR_DATA BIT(0)
#define OVRR_ERR BIT(1)
#define PAR_ERR BIT(2)
#define FRAM_ERR BIT(3)
#define BRK_INT BIT(4)
#define THR_EMPTY BIT(5)
#define TR_EMPTY BIT(6)
#define FIFO_ERR BIT(7)

/*
 * @brief Interrupt Enable Register
 */
#define EN_RDA_INT BIT(0)
#define EN_THRE_INT BIT(1)
#define EN_LS_INT BIT(2)
#define EN_MS_INT BIT(3)

/*
 * @brief Interrupt Identification Register
 */
#define NO_INT 0x01
#define INT_PEND 0x0E
#define RLS_INT_PEND 0x05
#define RD_INT_PEND 0x04
#define CT_INT_PEND 0x0C
#define THRE_INT_PEND 0x02
#define _INT_PEND 0x00

#define CHAR_TIMEOUT 0x08
#define SF_BYTE_FIFO 0x20
#define FCR_IS_SET 0xC0

/*
 * @brief FIFO Control Register
 */
#define EN_BOTH_FIFO BIT(0)
#define CLR_RCVR_FIFO BIT(1)
#define CLR_TRSM_FIFO BIT(2)
#define EN_64_BYTES_FIFO BIT(5)
#define RCVR_FIFO_TRGGR_LVL 0xC0
#define TRGGR_LVL_1 0x00
#define TRGGR_LVL_4 0x40
#define TRGGR_LVL_8 0x80
#define TRGGR_LVL_16 0xC0

typedef enum {
	no_int, int_receiver_line_status, int_rec_data, int_char_timeout, int_thr_empty, int_modem_status
} interrupt_id_t;

typedef enum {
	no_err, overrun_err, parity_err, framing_err, break_interrupt, receiver_data, thr_empty
} lsr_state_t;

#define SER_BIT_RATE 9600

#endif
