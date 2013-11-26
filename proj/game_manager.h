#ifndef __GAME_MANAGER_H
#define __GAME_MANAGER_H

#include "devices.h"

/** @defgroup game_manager game_manager
/* @{
 *
 * Classe que gere o sistema de jogo
 */

extern unsigned long long timer0_counter;

/*
 * @brief Cria os fighter e carrega inicialmente os sprites todos para memoria e guarda a informacao nos fighters
 *
 * @return 0 em caso de sucesso. Em caso de erro, a funcao handle_exit() e invocada, desativando as interrupcoes e o modo grafico
 */
int load_game();

/*
 *@brief Inicia o menu. Desenha o background, o cursor e invoca as funcoes que gerem as interrupcoes do menu.
 *
 *@return 0
 */
int startup_game();

/*
 * @brief Atualiza o ecra. Desenha o background, as posicoes atuais dos fighters, as vidas, o tempo da ronda e a mensagem de aviso, caso tenha passado o tempo definido.
 *
 * @return 0
 */
int update_display();

/*
 * @brief Funcao invocada para gerir situacoes de erro. Desenha imagem final, desativa interrupcoes e desativa o modo grafico.
 */
void handle_exit();

/**@}*/
#endif
