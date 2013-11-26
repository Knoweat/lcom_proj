#include "keyboard.h"
#include "devices.h"

#define DELAY_US 20000

extern unsigned int hook_id_keyboard;
extern unsigned int irq_set_keyboard;

long scancode = 0x00;

int keyboard_subscribe_int(void)
{
	hook_id_keyboard = 1;
	if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&hook_id_keyboard) != OK)
	{
		printf("keyboard_subscribe_int: sys_irqsetpolicy() failed\n");
		return -1;
	}
	return 0;
}

int keyboard_unsubscribe_int()
{

	if (sys_irqrmpolicy(&hook_id_keyboard) != OK)
	{
		return -1;
	}
	return 0;
}

long read_scan_code()
{
	long tmp;
	sys_inb(OUT_BUF, &tmp);
	return tmp;
}

long read_kbc_data()
{

	long stat, data;

	while (1)
	{
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */
		if (stat & OBF)
		{
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ((stat & (M_PAR_ERR | M_TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int issue_kbd_command(long cmd, long arg)
{
	long stat;

	keyboard_subscribe_int();
	while (1)
	{
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0)
		{
			sys_outb(IN_BUF, cmd);
			long response = read_kbd_ret_val();
			if (response == RESEND)
				;
			else if (response == ERROR)
			{
				printf("issue_kbd_command: command error\n");
				return -1;
			}
			else if (response == ACK)
			{
				break;
			}
			else
			{
				printf("read_kbd_ret_val: error\n");
			}
		}
		if (arg & 0x80)
			return 0;
		else
		{
			sys_inb(STAT_REG, &stat);
			if ((stat & IBF) == 0)
			{
				sys_outb(IN_BUF, arg);
				long response_arg = read_kbd_ret_val();
				if (response_arg == RESEND)
					;
				else if (response_arg == ERROR)
				{
					printf("issue_kbd_command: arg error\n");
					keyboard_unsubscribe_int();
					return -1;
				}
				else if (response_arg == ACK)
				{
					break;
				}
				else
				{
					printf("read_kbd_ret_val: error\n");
				}
			}
		}
	}
	keyboard_unsubscribe_int();
	return 0;
}

long read_kbd_ret_val()
{
	unsigned int i = 0;
	long data, stat;

	do
	{
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(STAT_REG, &stat);
		if ((stat & M_TO_ERR) == 0)
		{
			sys_inb(OUT_BUF, &data);
			return data;
		}
		i++;
	} while (i < 20);
	return 0x00;
}

int switch_leds(char leds_arg)
{
	issue_kbd_command(SWITCH_LEDS, leds_arg);
}
