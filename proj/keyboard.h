#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

/** @defgroup keyboard keyboard
/* @{
 *
 * Classe que gere o teclado
 */

extern long scancode;

/*
 * @brief Subscreve as interrupcoes do keyboard.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int keyboard_subscribe_int();

/*
 * @brief Desativa as interrupcoes do keyboard.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int keyboard_unsubscribe_int();

/*
 * @brief Le o scancode de uma tecla.
 *
 * @return scancode da tecla pretendida
 */
long read_scan_code();

/*
 * @brief Le a informacao do buffer do kbc.
 *
 * @return dados do buffer em caso de sucesso, -1 em caso de erro
 */
long read_kbc_data();

/*
 * Envia um comando para o keyboard.
 *
 * @param cmd comando que se pretende enviar
 * @param arg  informacao que se pretende escrever
 */
int issue_kbd_command(long cmd, long arg); /* use bit 7 set to 1 if no arg */

/*
 * @brief Le o valor de retorno do keyboard
 *
 * @return informacao retornada pelo kbd em caso de sucesso, 0 em caso de erro
 */
long read_kbd_ret_val();

/*
 * @brief Troca os LEDS.
 *
 * @param leds_arg LEDS que se pretende ativar/desativar
 */
int switch_leds(char leds_arg);

/**@}*/
#endif
