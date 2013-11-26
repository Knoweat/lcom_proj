#ifndef __SPEAKER_H
#define __SPEAKER_H

typedef struct
{
	unsigned long freq;
	unsigned long time;
} note_t;

/** @defgroup speaker speaker
 * @{
 *
 * Functions that control the speaker output
 */

/**
 * @brief Ativa/desativa o speaker
 *
 * @param on Se 0 desativa o speaker, caso contrario ativa
 *
 * @return 0
 */
int speaker_ctrl(unsigned char on);

/*
 * @brief Toca musica de fundo durante o jogo. Verifica se o tempo da nota ja passou, se sim, toca a proxima nota, com a frequencia indicada.
 *
 * @return 0
 */
int play_music();
/**@}*/

#endif /* __SPEAKER_H */
