#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"

/** @defgroup game_manager game_manager
/* @{
 *
 * Classe que gere o rato
 */

/*
 * @brief Subscreve as interrupcoes do mouse.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int mouse_subscribe_int();

/*
 * @brief Desativa as interrupcoes do mouse.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int mouse_unsubscribe_int();

/*
 * @brief Inicializa o envio dos packets do mouse.
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int kbc_init();

/*
 * @brief Envia comandos para o keyboard controller.
 *
 * @param cmd comando que se pretende enviar
 * @param argc quantidade de informacao que se pretende enviar (tamanho de argv)
 * @param array de informacao que se pretende enviar
 */
int issue_kbc_command(long cmd, int argc, long *argv);

/*
 *@brief Le o valor de retorn do kbc.
 *
 *@return informacao retornada pelo kbc em caso de sucesso, 1 em caso de erro
 */
long read_func_ret_val();

/*
 * @brief Esvazia o buffer ate o buffer poder receber novos packets
 */
void flush();

/*
 * @brief Verifica se o algum botao do mouse esta pressionado
 *
 * @param byte correspondente a informacao dos botoes (1o byte do packet)
 */
int pressed_button(unsigned char byte);

/*
 * @brief Lê um pacote do rato, e atualiza a variável global packet_nr.
 */
void receive_mouse_packet();

/**@}*/
#endif
