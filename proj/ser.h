#ifndef _SER_H
#define _SER_H

#include "utils.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "serial_consts.h"

/** @defgroup serial serial
/* @{
 *
 * Classe que gere a porta-serie
 */

unsigned long ser_read_ctrl_reg(unsigned long com_port);
unsigned long ser_get_bit_rate(unsigned long com_port);
int ser_set_bit_rate(unsigned long com_port, unsigned long bit_rate);
int ser_write_ctrl_reg(unsigned long com_port, unsigned long data);
lsr_state_t read_lsr_state(unsigned long com_port);

int disable_ser_interrupts(unsigned long com_port);
int handle_ser_event(unsigned long com_port);
unsigned char ser_read_char_poll(unsigned long com_port);
int ser_write_char_poll(unsigned long com_port, unsigned char snd_chr);

/*
 * @brief Subscreve as interrupcoes da serial port.
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int ser_subscribe_int(unsigned long com_port);

/*
 * @brief Desativa as interrupcoes da serial port.
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int ser_unsubscribe_int(unsigned long com_port);

/*
 * @brief Gere as interrupcoes da serial port no pc que recebe a informacao
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return char recebido em caso de sucesso, diferente de 0 em caso de erro
 */
unsigned char ser_int_handler_rec(unsigned long com_port);

/*
 * @brief Gere as interrupcoes da serial port no pc que transmite a informacao
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return diferente de 0 em caso de erro
 */
int ser_int_handler_trsm(unsigned long com_port, unsigned char chr);

/*
 * @brief Ativa as interrupçoes de Received Data Available e de Line Status no pc que recebe a informação,
 * e ativa as interrupçoes de Transmitter Holding Register Empty no pc que transmite a informação
 *
 * @param com_port porta com que se pretende comunicar
 * @param tx 0 se receiver, 1 se sender
 *
 * @return diferente de 0 em caso de erro
 */
int enable_ser_interrupts(unsigned long com_port, unsigned char tx);

/*
 * @brief Verifica qual a interrupção de maior prioridade.
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return a identificação da interrupção, através de um objeto do tipo interrupt_id_t
 */
interrupt_id_t find_int_id(unsigned long com_port);

/*
 * @brief Função chamada aquando de uma interrupção da serial port no pc de receção.
 *
 * @param com_port porta com que se pretende comunicar
 *
 * @return o carater lido, apenas caso nao existam error na linha
 */
unsigned char handle_ser_event_rec(unsigned long com_port);

/*
 * @brief Função chamada aquando de uma interrupção da serial port no pc de envio.
 *
 * @param com_port porta com que se pretende comunicar
 * @param trsm_chr carater a transmitir
 *
 * @return diferente de 0 em caso de erro
 */
int handle_ser_event_trsm(unsigned long com_port, unsigned char trsm_chr);

/**@}*/
#endif
