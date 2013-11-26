#ifndef __TIMER_H
#define __TIMER_H

#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "utils.h"

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Configura o timer para gerar um square wave
 * 
 * @param timer timer a configurar (de 0 a 2)
 * @param freq frequencia da square wave a gerar
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int timer_set_square(unsigned long timer, unsigned long freq);

/**
 * @brief Subscreve as interrupcoes do timer 0.
 *
 * @return retorna 0 em caso de sucesso, -1 em caso de erro
 */
int timer_subscribe_int(void );

/**
 * @brief Desativa as interrupcoes do timer 0.
 *
 * @return retorna 0 em caso de sucesso, -1 em caso de erro
 */
int timer_unsubscribe_int();

/**
 * @brief Gera as interrupcoes do timer. Incrementa a variavel do tempo.
 */
void timer_int_handler();

/*
 * @brief Gera as interrupcoes do timer. Incrementa o contador passado como parametro.
 *
 * @param counter contador a ser incrementado
 */
int hancle_timer0_evt(unsigned long long *counter);

/**@}*/
#endif /* __TIMER_H */
