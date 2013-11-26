#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "vbe.h"
#include "constants.h"
#include "fighter.h"
#include "sprites.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/** @name Video Graphics Mode Information */
/**@{*/
typedef struct
{

	/** @name VRAM info */
	/**@{*/
	unsigned long vram_size; /**< @brief VRAM size in 16-bit words*/
	phys_bytes vram_base_phys; /**< @brief VRAM base physical address. */
	void * vram_base_virt;
	/**@}*/
	/** @name Screen parameters */
	/**@{*/
	unsigned scr_xres; /**< @brief horizontal resolution */
	unsigned scr_yres; /**< @brief vertical resolution */
	unsigned bits_per_pixel; /**< @brief bytes used in each pixel */
	/**@}*/
} vg_info_gr;
/**@}*/

/**
 * @brief Inicializa o modo grafico
 *
 *
 * Usa o VBE INT 0x10 interface para ativar o modo grafico desejado, mapeia a VRAM para o enderecamento de espaco do processo e inicializa as variaveis globais com a resolucao do ecra e o numero de cores.
 * 
 * @param mode modo 16-bit VBE
 * 
 * @return endereco virtual onde a VRAM foi mapeada. NULL em caso de erro
 */
void *vg_init(unsigned short mode);

/**
 * @brief Preenche o ecra com determinada cor
 * 
 * @param color cor a preencher o ecra
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int vg_fill(unsigned long color);

/**
 * @brief Pinta um determinado pixel com a cor especificada
 * 
 * Pinta a cor do pixel na posicao especificada, escrevendo na posicao da VRAM correspondente
 * 
 * @param x coordenada no eixo dos x
 * @param y coordenada no eixo dos y
 * @param color cor que se pretende pintar o pixel
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color);

/*
 * @brief Mesma funcao que vg_set_pixel, mas nao escreve o pixel caso seja da cor 0xFF00FF (RGB 8:8:8)
 *
 * @param x coordenada no eixo dos x
 * @param y coordenada no eixo dos y
 * @param color cor que se pretende pintar o pixel
 *
 * @return 0 em caso de sucesso, -1 em caso de erro
 *
 */
int vg_set_pixel_sprite(unsigned long x, unsigned long y, unsigned long color);

/**
 * @brief Returns the color of the input pixel
 * 
 * Returns the color of the pixel at the specified position, 
 *  by reading to the corresponding location in VRAM
 * 
 * @param x horizontal coordinate, starts at 0 (leftmost pixel)
 * @param y vertical coordinate, starts at 0 (top pixel)
 * @return color of the pixel at coordinates (x,y), or -1 if some input argument is not valid
 */
unsigned long vg_get_pixel(unsigned long x, unsigned long y);

/*
 * @brief Desenha o sprite.
 *
 * @param sprite sprite que se deseja desenhar
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int vg_draw_sprite(sprite_t *sprite);

/*
 * @brief Desenha o background. As dimensoes estao definidas para o modo usado, 800x600.
 *
 * @param background sprite correspondente ao background
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int vg_draw_background(sprite_t * background);

/**
 * @brief Volta ao modo standard do Minix3 (0x03: 25 x 80, 16 cores)
 * 
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int vg_exit(void);

#endif /* __VIDEO_GR_H */
