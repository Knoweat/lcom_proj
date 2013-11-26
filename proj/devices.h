#ifndef __DEVICES_H
#define __DEVICES_H

#include "constants.h"
#include "timer.h"
#include "mouse.h"
#include "keyboard.h"
#include "sprites.h"
#include "video_gr.h"
#include "rtc.h"
#include "rtc_consts.h"
#include "ser.h"
#include "serial_consts.h"
#include "i8254.h"
#include "i8042.h"
#include "fighter.h"

/** @defgroup video_gr video_gr

/** @name event_t */
/**@{*/
typedef enum
{
	timer0_evt, keyboard_evt, mouse_evt, rtc_evt, serial_evt
} event_t;
/**@}*/

extern unsigned int hook_id_timer0;
extern unsigned int hook_id_keyboard;
extern unsigned int hook_id_mouse;
extern unsigned int hook_id_rtc;
extern unsigned int hook_id_serial;

extern unsigned int irq_set_timer0;
extern unsigned int irq_set_keyboard;
extern unsigned int irq_set_mouse;
extern unsigned int irq_set_rtc;
extern unsigned int irq_set_serial;



int init_game_devices();
/*
 * @brief Subscreve as interrupcoes necessarias para o menu (mouse, rtc, serial)
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int init_startup_devices();
/*
 * @brief Desativa as interrupcoes usadas no menu
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int term_startup_devices();
/*
 * @brief Gere as interrupcoes do menu. Desenha o background e o cursor.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int handle_startup_ints();
/*
 * @brief Gere as interrupcoes do mouse no menu. Verifica se esta o botao a ser clicado numa das duas opcoes disponiveis (1PC ou 2PC).
 *
 * @return 1 em caso esteja a clicar numa opcao, 0 caso contrario
 */
int handle_startup_mouse_evt();
/*
 * @brief Gere as interrupcoes do RTC no menu.
 *
 * @return 0
 */
int handle_startup_rtc_evt();
/*
 * @brief Gere as interrupcoes da serial no menu
 *
 * @return 1 caso o utilizador pretenda jogar em 2PC, 0 caso contrario
 */
int handle_startup_serial_evt();

/*
 * @brief Subscreve as interrupcoes necessarias para as rondas (mouse, keyboard, timer, RTC, serial)
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int init_round_devices();
/*
 * @brief Desativa as interrupcoes usadas nas rondas
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int term_round_devices();
/*
 * @brief Gere as interrupcoes necessarias das rondas. Atualiza os fighters. Termina quando acaba o tempo-limite da ronda ou um dos fighters morre.
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int handle_round_ints();
/*
 * @brief Gere as interrupcoes to timer nas rondas. Incrementa a variavel do tempo (timer0_counter)
 *
 * @return 1 caso tenha terminado o tempo da ronda, 0 caso contrario
 */
int handle_round_timer0_evt();
/*
 * @brief Gere as interrupcoes do keyboard nas rondas. Verifica qual a tecla premida e executa a respetiva acao. Caso esteja em modo 2PCs, envia tambem a informacao para o outro PC em modo polling.
 *
 * @return 0
 */
int handle_round_keyboard_evt();
/*
 * @brief Gere as interrupcoes do mouse nas rondas. Verifica as deslocacoes do rato e executa a respetiva acao. Caso esteja em modo 2PCs, envia tambem a informacao para o outro PC em modo polling.
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int handle_round_mouse_evt();
/*
 * @brief Gere as interrupcoes do RTC nas rondas. Toca a musica de fundo e incrementa a variavel de tempo (rtc_counter)
 *
 * @return 0
 */
int handle_round_rtc_evt();
/*
 * @brief Gere as interrupcoes da serial nas rondas. Verifica o char recebido e executa a respetiva acao.
 *
 * @return 0
 */
int handle_round_serial_evt();

#endif
