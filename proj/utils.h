#ifndef __UTILS_H
#define __UTILS_H

#define BIT(n) (1<<n)

/** @defgroup utils utils
/* @{
 *
 * Funcoes uteis
 */

/**
 * @brief Converte do formato BCD para decimal.
 *
 * @param val valor que se pretende converter
 * @return retorna valor convertido em decimal
 */
unsigned long BCDToDec(unsigned long val);

/**
 * @brief Converte de decimal para formato BCD.
 *
 * @param valor que se pretende converter
 * @return retorna valor convertido em BCD
 */
unsigned long DecToBCD(unsigned long val);

/**@}*/
#endif
