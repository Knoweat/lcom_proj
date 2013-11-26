#ifndef __SPRITES_H
#define __SPRITES_H

#include "constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @defgroup video_gr video_gr

/** @name Sprite */
/**@{*/
typedef struct Sprite
{
	unsigned long x_init_pos; /**< @brief posicao inicial do sprite no eixo dos x*/
	unsigned long y_init_pos; /**< @brief posicao inicial do sprite no eixo dos y*/
	unsigned long x_dim;	/**< @brief comprimento do sprite*/
	unsigned long y_dim;	/**< @brief altura do sprite*/
	unsigned char *ptr;		/**< @brief apontador para a informacao do sprite*/
} sprite_t;
/**@}*/

/** @name Color */
/**@{*/
typedef struct Color
{
	unsigned int nr_chars;	/**< @brief numero de chars que representam a cor*/
	unsigned char *char_seq;	/**< @brief sequencia de chars que representam a cor*/
	unsigned char col[2];	/**< @brief array de 2 bytes que representa a cor convertida em 5:6:5*/
} color_t;
/**@}*/

/*
 * @brief Carrega o sprite para um objeto sprite_t, com as coordenadas indicadas.
 *
 * @param sprite_f  sprite que ja contem a informacao armazenada pronta para ser lida
 * @param sprite local onde vai ser armazenado a informacao do sprite
 * @param x_ini coordenadas x do sprite
 * @param y_ini coordenadas y do sprite
 *
 */
int load_sprite(unsigned char *sprite_f[], sprite_t * sprite,
		unsigned long x_ini, unsigned long y_ini);

/*
 * @brief Converte RGB 8:8:8 para RGB 5:6:5.
 *
 * @param ptr onde e armazenada a cor convertida
 * @param red codigo do vermelho da cor no formato 8:8:8
 * @param green codigo do verde da cor no formato 8:8:8
 * @param blue codigo do azul da cor no formato 8:8:8
 */
int convert_888_to_565(unsigned char *ptr, unsigned long red,
		unsigned long green, unsigned long blue);

/*
 * @brief Le um ficheiro de texto, convertido de formato xpm, e carrega para uma struct do tipo sprite_t.
 *
 * @param file_name nome do ficheiro que se pretende ler
 * @param sprite_f struct onde se pretende guardar a informacao
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int load_to_strings(unsigned char *file_name, unsigned char ***sprite_f);

/*
 * @brief Desenhar a barra da vida. Desenha 2 pixeis por cada ponto de vida. Para diferentes vidas, pode alterar a cor da barra.
 *
 * @param pos_x posicao inicial no eixo dos x da barra (pode ser alterada em constants.h)
 * @param pos_y posicao inicial no eixo dos y da barra (pode ser alterada em constants.h)
 * @param hp vida do fighter
 */
int draw_life(int pos_x, int pos_y, int hp);

/*
 * @brief Desenha um numero. Usado para a contabilizacao das rondas e do tempo das rondas.
 *
 * @param pos_x posicao inicial no eixo dos x da barra
 * @param pos_y posicao inicial no eixo dos y da barra
 * @param nr numero que se pretende desenhar
 */
int draw_nr(int pos_x, int pos_y, int nr);

/*
 * @brief Verifica a colisao entre 2 sprites. Existe colisao quando 2 pixeis que nao sejam da cor predefinida (FF00FF em RGB8:8:8, definida em constants.h) se sobrepoem.
 *
 * @param player1 fighter do lado esquerdo
 * @param player2 fighter do lado direito
 *
 * @return 0 caso nao haja colisao, 1 caso haja.
 */
int check_colision(sprite_t* player1, sprite_t* player2);

#endif
