#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>

#include "mouse.h"
#include "i8042.h"
#include "timer.h"
#include "devices.h"
#include "keyboard.h"

#define DELAY_US 25000
#define DELAY_RESET 500000

extern unsigned int hook_id_mouse;
extern unsigned int hook_id_keyboard;
extern unsigned int irq_set_mouse;

long packet[ELEMS_MOUSE_PACKET] =
{ 0, 0, 0 };
unsigned short i = 0;
static int is_mouse_sync = 1;
static int nr_tries = 0;

unsigned int counter = 0;

int mouse_subscribe_int(void)
{
	hook_id_mouse = 2;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&hook_id_mouse) != OK)
	{
		printf("mouse_subscribe_int: sys_irqsetpolicy() failed\n");
		return -1;
	}
	return 0;
}

int mouse_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id_mouse) != OK)
	{
		printf("mouse_subscribe_int: sys_irqrmpolicy() failed\n");
		return -1;
	}
	return 0;
}

int pressed_button(unsigned char byte)
{
	if (byte & BIT(0) || byte & BIT(1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int kbc_init()
{
	unsigned long ptr[3] =
	{ 0, 0, 0 };
	/*
	 issue_kbc_command(ENABLE_MOUSE, 0, ptr);
	 while (1)
	 {
	 long data;
	 ptr[0] = RESET;
	 issue_kbc_command(WRITE_B, 1, ptr);
	 tickdelay(micros_to_ticks(DELAY_RESET));
	 if (read_func_ret_val() == MOUSE_OK)
	 {
	 if (read_func_ret_val() == DEVICE_ID)
	 {
	 ptr[0] = DISABLE_STREAM_M;
	 if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
	 return 1;
	 ptr[0] = SET_SAMPLE_RATE;
	 ptr[1] = DEFAULT_SAMPLE_RATE;
	 if (issue_kbc_command(WRITE_B, 2, ptr) != OK)
	 return 1;
	 ptr[0] = SET_RESOLUTION;
	 ptr[1] = DEFAULT_RESOLUTION;
	 if (issue_kbc_command(WRITE_B, 2, ptr) != OK)
	 return 1;
	 ptr[0] = SET_SCALING;
	 if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
	 return 1;
	 if (issue_kbc_command(READ_CMD_BYTE, 0, ptr) != OK)
	 return 1;
	 data = read_func_ret_val();
	 data &= EN_KBD_MOUSE;
	 data |= EN_INT_KBD_MOUSE;
	 ptr[0] = data;
	 printf("data:0x%x\n", data);
	 if (issue_kbc_command(WRITE_CMD_BYTE, 1, ptr) != OK)
	 return 1;*/
	ptr[0] = EN_SEND_PACK;
	if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
		return 1;
	/*ptr[0] = ENABLE_STREAM_M;
	 if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
	 return 1;*/
	flush();
	return 0;
	/*}
	 }
	 }
	 return 1;*/
}

int get_mouse_stat()
{
	mouse_subscribe_int();
	keyboard_subscribe_int();
	long ptr[3] =
	{ 0, 0, 0 };
	ptr[0] = DISABLE_STREAM_M;
	if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
		return 1;

	flush();
	ptr[0] = STAT_REQUEST;
	if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
		return 1;
	i = 0;
	while (i < 3)
	{
		sys_inb(DATA_REG, &packet[i]); // = read_func_ret_val();
		/*if ((packet[0] & BIT(7))|| (packet[0] & BIT(3)));
		 else
		 {*/
		i++;
		//}
	}

	mouse_unsubscribe_int();
	keyboard_unsubscribe_int();
	return 0;
}

int issue_kbc_command(long cmd, int argc, long *argv)
{

	long stat;
	issue_cmd: sys_outb(KBC_CMD_REG, cmd);

	while (1)
	{
		int i = 0;
		while (i < argc)
		{
			sys_inb(STAT_REG, &stat);
			if ((stat & IBF) == 0)
			{
				sys_outb(DATA_REG, argv[i]);
				response: ;
				long response_arg = read_func_ret_val();
				if (response_arg == NACK)
				{
					goto issue_cmd;
				}
				else if (response_arg == ERROR)
				{
					printf("issue_kbd_command: arg error\n");

					return 1;
				}
				else if (response_arg == ACK)
				{
					i++;
				}
				else
				{
					goto response;
				}
			}
		}
		if (i == argc)
		{
			return 0;
		}
	}
}

long read_func_ret_val()
{
	unsigned int i = 0;
	long data, stat;

	do
	{
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(STAT_REG, &stat);
		if (stat & OBF)
		{
			sys_inb(OUT_BUF, &data);

			if ((stat & M_TO_ERR) == 0)
			{
				return data;
			}
			else
				return 1;
		}
		i++;
	} while (i < 20);
	return 0;
}

void flush()
{
	long data, stat;

	while (1)
	{
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(STAT_REG, &stat);
		if (stat & OBF)
		{
			sys_inb(OUT_BUF, &data);
		}
		else
		{
			return;
		}
	}
}
