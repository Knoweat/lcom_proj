#include "utils.h"

unsigned long BCDToDec(unsigned long val)
{
	unsigned int i = 2; /*> nr of algarisms in a single BCD byte */
	unsigned long mask = 0x0F;
	unsigned long tmp = 0;
	unsigned int mult = 1;

	while (i > 0)
	{
		tmp += mult * (mask & val);
		val >>= 4;
		if (i > 0)
			mult *= 10;
		i--;
	}
	return tmp;
}

unsigned long DecToBCD(unsigned long val)
{
	unsigned int a = val % 10;
	unsigned int tmp = a;
	val /= 10;
	a = val % 10;
	tmp |= (a << 4);
	return tmp;
}
