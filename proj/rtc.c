
#include "rtc.h"

extern unsigned int hook_id_rtc;
extern unsigned int irq_set_rtc;

int rtc_subscribe_int(int exclusive)
{
	hook_id_rtc = 3;
	if (exclusive)
	{
		if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id_rtc) != OK)
		{
			printf("rtc_subscribe_int: sys_irqsetpolicy() failed\n");
			return -1;
		}
	}
	else
	{
		if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
				&hook_id_rtc) != OK)
		{
			printf("rtc_subscribe_int: sys_irqsetpolicy() failed\n");
			return -1;
		}
	}
	return 0;
}

int rtc_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id_rtc) != OK)
	{
		printf("rtc_unsubscribe_int: sys_irqrmpolicy() failed\n");
		return -1;
	}
	return 0;
}

unsigned long read_rtc_reg(unsigned long internal_reg)
{
	unsigned long data = 0;

	sys_outb(RTC_ADDR_REG, internal_reg);
	sys_inb(RTC_DATA_REG, &data);
	return data;
}

unsigned char handle_rtc_event()
{
	unsigned long data = read_rtc_reg(REG_C);
	unsigned char ints = 0;
	if (data & AF)
		ints |= BIT(alarm_int);
	if (data & UF)
		ints |= BIT(update_int);
	if (data & PF)
		ints |= BIT(periodic_int);
	return ints;
}

int enable_ints(unsigned char ints)
{
	unsigned long data = read_rtc_reg(REG_B);
	if (ints & BIT(alarm_int))
		data |= AIE;
	else
		data &= ~AIE;
	if (ints & BIT(update_int))
		data |= UIE;
	else
		data &= ~UIE;
	if (ints & BIT(periodic_int))
		data |= PIE;
	else
		data &= ~PIE;
	write_rtc_reg(REG_B, data);
	data = read_rtc_reg(REG_C);
	data |= 0x80;
	write_rtc_reg(REG_C, data);

	return 0;
}

int disable_ints(unsigned char ints)
{
	unsigned long data = read_rtc_reg(REG_B);
	if (ints & BIT(alarm_int))
		data &= ~AIE;
	if (ints & BIT(update_int))
		data &= ~UIE;
	if (ints & BIT(periodic_int))
		data &= ~PIE;
	write_rtc_reg(REG_B, data);
	data = read_rtc_reg(REG_C);
	data &= 0x7F;
	write_rtc_reg(REG_C, data);
}

int write_rtc_reg(unsigned long internal_reg, unsigned long byte)
{
	sys_outb(RTC_ADDR_REG, internal_reg);
	sys_outb(RTC_DATA_REG, byte);
	return 0;
}

int init_rtc_read()
{
	unsigned long reg = 0, regA = 0, regB = 0;
	do
	{
		rtc_subscribe_int(0);
		sys_outb(RTC_ADDR_REG, REG_A);
		sys_inb(RTC_DATA_REG, &regA);
		rtc_unsubscribe_int();
	} while (regA & UIP);

	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	regB |= SET;
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_outb(RTC_DATA_REG, regB);

	return 0;
}

int finish_rtc_read()
{
	unsigned long regB = 0;
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_inb(RTC_DATA_REG, &regB);
	regB &= ~SET;
	sys_outb(RTC_ADDR_REG, REG_B);
	sys_outb(RTC_DATA_REG, regB);
	return 0;
}

int rtc_enable_square_wave()
{
	unsigned long data = read_rtc_reg(REG_B);
	data |= SQWE;
	write_rtc_reg(REG_B, data);
	data = read_rtc_reg(REG_A);
	data &= 0xF0;
	data |= 0x06;
	write_rtc_reg(REG_A, data);
	return 0;
}

int set_alarm(unsigned long delta_s)
{
	init_rtc_read();

	unsigned long curr_seconds = read_rtc_reg(SECONDS_R);
	unsigned long curr_minutes = read_rtc_reg(MINUTES_R);
	unsigned long curr_hours = read_rtc_reg(HOURS_R);
	curr_seconds = BCDToDec(curr_seconds);
	curr_minutes = BCDToDec(curr_minutes);
	curr_hours = BCDToDec(curr_hours) - 1;

	curr_seconds = curr_seconds + delta_s;
	curr_minutes = curr_minutes + (curr_seconds / 60);
	curr_hours = curr_hours + (curr_minutes / 60);
	curr_minutes %= 60;
	curr_seconds %= 60;
	curr_hours %= 24;
#ifdef DEBUG
	printf("hours %d mins %d secs %d\n", curr_hours, curr_minutes, curr_seconds);
#endif
	write_rtc_reg(HOURS_ALARM_R, DecToBCD(curr_hours + 1));
	write_rtc_reg(MINUTES_ALARM_R, DecToBCD(curr_minutes));
	write_rtc_reg(SECONDS_ALARM_R, DecToBCD(curr_seconds));
	finish_rtc_read();
	return 0;
}
