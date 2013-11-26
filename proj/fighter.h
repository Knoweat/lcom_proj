#ifndef __FIGHTER_H
#define __FIGHTER_H

#include "constants.h"
#include "devices.h"
#include "sprites.h"

/** @defgroup fighter fighter
/* @{
 *
 * Classe que representa um personagem do jogo
 */

/** @name stance_t */
/**@{*/
typedef enum
{
	stand1,
	stand2,
	walk1,
	walk2,
	walk3,
	walk4,
	LA1,
	LA2,
	MA1,
	MA2,
	HA1,
	HA2,
	LD,
	MD,
	HD,
	Win,
	Die1,
	Die2,
	Hit
} stance_t;
/**@}*/

/** @name fighter_t */
/**@{*/
typedef struct
{
	int not_verified;
	int pos_x;	/**< @brief posicao no eixo dos x */
	int victories;	/**< @brief numero de vitorias */
	int pos_y;	/**< @brief posicao no eixo dos y */
	int life;	/**< @brief vida */
	int player_number; 	/**< @brief numero do jogador - pode ser 1 ou 2 */
	sprite_t waiting_stance[2]; 	/**< @brief sprites de 'espera' */
	sprite_t low_attack_stance[2];	/**< @brief sprites de ataque baixo */
	sprite_t medium_attack_stance[2];	/**< @brief sprites de ataque medio */
	sprite_t high_attack_stance[2];	/**< @brief sprites de ataque alto */
	sprite_t low_defense_stance [1]; 	/**< @brief sprites de defesa baixa */
	sprite_t medium_defense_stance [1];	/**< @brief sprites de defesa media */
	sprite_t high_defense_stance [1];	/**< @brief sprites de defesa alta */
	sprite_t walking_stance[4];		/**< @brief sprites de movimento */
	sprite_t die_stance[2];	/**< @brief sprites de morte */
	sprite_t win_stance [1];	/**< @brief sprites de vencer a ronda*/
	sprite_t hit_stance [1];	/**< @brief sprites de sofrer dano */
	unsigned long long time_begin_action;	/**< @brief tempo em que comecou a ultima acao */
	float power; /**< @brief numero entre 0 e 1 que determina a forca */
	float defense; /**< @brief numero entre 0 e 1 que determina a defesa */
	stance_t stance;	/**< @brief estado em que se encontra atualmente */
	int movement; 	/**< @brief -1 se esta a mover para tras, 0 se nao esta a mover, 1 se esta a mover para a frente */

} fighter;
/**@}*/

/*
 * @brief Inicia os fighters nas posicoes iniciais com os estados e vida iniciais para comecar uma nova ronda.
 *
 * @param f1 fighter1
 * @param f2 fighter2
 */
void init_fighter(fighter* f1, fighter* f2);

/*
 * @brief Cria os fighters nas posicoes iniciais. Caso seja o jogador 1, cria numa posicao. se for o 2, cria noutra. (As posicoes podem ser alteradas em constants.h)
 *
 * @param number numbero do fighter (pode ser 1 ou 2)
 * @param power valor entre 0 e 1 que indica a 'forca' do fighter
 * @param defense valor entre 0 e 1 que indica a 'defesa' do fighter
 *
 * @return fighter que foi criado
 */
fighter* create_fighter(int number, float power, float defense);

/*
 * @brief Movimenta o jogador para a frente. Verifica se ha colisao com o adversario. Se houver, vai decrementando a distancia que percorre ate nao existir colisao.
 *
 * @param f_move fighter que se pretende mover
 * @param f adversario (preciso passar como parametro para usar no check_colision)
 *
 * @return 0
 */
int move_front(fighter* f_move, fighter* f); // atualizar o time_begin_action e next_action. se houver colisao, da os passos sem se mexer

/*
 * @brief Movimenta o jogador para tras. Verifica se ultrapassa os limites do ecra. Se sim, atualiza os valores do sprite com o minimo/maximo valor que o fighter pode estar.
 *
 * @param f_move fighter que se pretende mover
 *
 * @return 0
 */
int move_back(fighter* f_move);

/*
 * @brief Atualiza a vida do fighter
 *
 * @param f fighter que se pretende atualizar a vida
 * @param lost_hp vida perdida resultante de um ataque do adversario
 */
void update_life(fighter* f, int lost_hp);

/*
 * @brief Atualiza o estado seguinte do jogador e a variavel time_begin_action para o tempo em que a acao comecou. Verifica se ha colisoes nos ataques e defesas do adversario, calculando assim a vida perdida.
 *
 *@param f1 fighter que se pretende atualizar
 *@param f2 fighter adversario (preciso passar como parametro para usar no check_colision)
 */
void update_fighter(fighter* f1, fighter* f2); //f1 e o player que se deseja atualizar

/*
 * @brief Atualiza a posicao dos sprites carregados inicialmente com a posicao atual do fighter
 *
 * @param f fighter que se pretende atualizar a posicao dos sprites
 */
void update_sprite_pos(fighter* f);

/*
 * @brief Retorna o sprite do estado atual em que o fighter se encontra
 *
 * @param f fighter que se pretende obter o sprite
 *
 * @return sprite correspondente ao estado atual do fighter
 */
sprite_t* get_curr_sprite(fighter* f);

/*
 * @brief Executa as acoes quando um fighter vence uma ronda. Incremente o numero de vitorias e atualiza o estado para 'Win'
 *
 * @param f fighter vencedor da ronda
 */
void win_round(fighter* f);

/*
 * @brief Executa as acoes de um ataque alto. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa o ataque alto
 */
void high_attack(fighter *f);

/*
 * @brief Executa as acoes de um ataque medio. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa o ataque medio
 */
void medium_attack(fighter *f);

/*
 * @brief Executa as acoes de um ataque baixo. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa o ataque baixo
 */
void low_attack(fighter *f);
/*
 * @brief Executa as acoes de uma defesa alta. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa a defesa alta
 */
void high_defence(fighter *f);

/*
 * @brief Executa as acoes de uma defesa media. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa a defesa media
 */
void medium_defence(fighter *f);

/*
 * @brief Executa as acoes de uma defesa baixa. Atualiza a variavel time_begin_action para o tempo inicial da acao e atualiza o estado do fighter
 *
 * @param f fighter que se executa a defesa baixa
 */
void low_defence(fighter *f);

/**@}*/

#endif
