#include "ser.h"
#include "devices.h"

extern unsigned int hook_id_serial;
extern unsigned int irq_set_serial;

unsigned int hook_id_com1;
unsigned int hook_id_com2;
unsigned int irq_set_com1;
unsigned int irq_set_com2;

unsigned long ser_read_ctrl_reg(unsigned long com_port)
{
	unsigned long data = 0;
	if (sys_inb(com_port + LC_REG, &data) != OK)
	{
		printf("ser_read_ctrl_reg: error reading LCR\n");
	}
	return data;
}

unsigned long ser_get_bit_rate(unsigned long com_port)
{
	unsigned long data = ser_read_ctrl_reg(com_port);
	data |= DLAB;
	ser_write_ctrl_reg(com_port, data);
	unsigned long bit_rate = 0;
	unsigned long tmp = 0;
	sys_inb(com_port + DLM_REG, &tmp);
	bit_rate |= tmp;
	bit_rate <<= 8;
	sys_inb(com_port + DLL_REG, &tmp);
	bit_rate |= tmp;
	data ^= DLAB;
	ser_write_ctrl_reg(com_port, data);
	bit_rate = BASE_FREQ / bit_rate;

	return bit_rate;
}

int ser_write_ctrl_reg(unsigned long com_port, unsigned long data)
{
	sys_outb(com_port + LC_REG, data);
	return 0;
}

int ser_set_bit_rate(unsigned long com_port, unsigned long bit_rate)
{
	bit_rate = BASE_FREQ / bit_rate;
	unsigned long msb = (bit_rate >> 8) & 0xFF;
	unsigned long lsb = bit_rate & 0xFF;

	unsigned long data = ser_read_ctrl_reg(com_port);
	data |= DLAB;
	ser_write_ctrl_reg(com_port, data);
	sys_outb(com_port + DLM_REG, msb);
	sys_outb(com_port + DLL_REG, lsb);
	data ^= DLAB;
	ser_write_ctrl_reg(com_port, data);
	return 0;
}

int is_data_available(unsigned long com_port)
{
	unsigned long data = 0;
	data = sys_inb(com_port + LS_REG, &data);
	return (data & RCVR_DATA);
}

int is_thr_empty(unsigned long com_port)
{
	unsigned long data = 0;
	sys_inb(com_port + LS_REG, &data);
	return (data & THR_EMPTY);
}

lsr_state_t read_lsr_state(unsigned long com_port)
{
	unsigned long data = 0;
	sys_inb(com_port + LS_REG, &data);
	if (data & OVRR_ERR)
		return overrun_err;
	else if (data & PAR_ERR)
		return parity_err;
	else if (data & FRAM_ERR)
		return framing_err;
	else if (data & BRK_INT)
		return break_interrupt;
	else if (data & RCVR_DATA)
		return receiver_data;
	else
		return no_err;
}

int disable_ser_interrupts(unsigned long com_port)
{
	unsigned long data = 0;
	sys_inb(com_port + IE_REG, &data);
	data &= 0xF8;
	sys_outb(com_port + IE_REG, &data);
	tickdelay(micros_to_ticks(UART_DELAY));
	return 0;
}

unsigned char ser_read_char_poll(unsigned long com_port)
{
	char rec_chr = 0;
	lsr_state_t lsr_state;
	do
	{
		lsr_state = read_lsr_state(com_port);
		unsigned long tmp = 0;
		switch (lsr_state)
		{
		case overrun_err:
			printf("overrun error\n");
			break;
		case parity_err:
			printf("parity error\n");
			break;
		case framing_err:
			printf("framing error\n");
			break;
		case break_interrupt:
			printf("break interruption\n");
			break;
		case receiver_data:
			sys_inb(com_port + RB_REG, &tmp);
			rec_chr = (tmp & 0xFF);
			tickdelay(micros_to_ticks(UART_DELAY));
			return rec_chr;
		}
	} while (lsr_state != receiver_data);
}

int ser_write_char_poll(unsigned long com_port, unsigned char snd_chr)
{
	while (!(is_thr_empty(com_port)))
		;
	sys_outb(com_port + TH_REG, snd_chr);
	return 0;
}

int ser_subscribe_int(unsigned long com_port)
{
	if (com_port == COM1_BASE_ADDR)
	{
		hook_id_com1 = hook_id_serial;
		if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
				&hook_id_com1) != OK)
		{
			printf("ser_subscribe_int: sys_irqsetpolicy() failed\n");
			return -1;
		}
	}

	else if (com_port == COM2_BASE_ADDR)
	{
		hook_id_com2 = 6;
		if (sys_irqsetpolicy(COM2_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
				&hook_id_com2) != OK)
		{
			printf("ser_subscribe_int: sys_irqsetpolicy() failed\n");
			return -1;
		}
	}
	else
	{
		printf("ser_subscribe_int: invalid com_port\n");
		return -1;
	}
	return 0;
}

int ser_unsubscribe_int(unsigned long com_port)
{
	if (com_port == COM1_BASE_ADDR)
	{
		if (sys_irqrmpolicy(&hook_id_com1) != OK)
		{
			printf("ser_unsubscribe_int: sys_irqrmpolicy() failed\n");
			return -1;
		}
	}
	else if (com_port == COM1_BASE_ADDR)
	{
		if (sys_irqrmpolicy(&hook_id_com2) != OK)
		{
			printf("ser_unsubscribe_int: sys_irqrmpolicy() failed\n");
			return -1;
		}
	}
	else
	{
		printf("ser_unsubscribe_int: invalid com_port\n");
		return -1;
	}
	return 0;
}

unsigned char ser_int_handler_rec(unsigned long com_port)
{
	int ipc_status;
	message msg;
	int r;
	char event = 0;
	unsigned int irq_set_com;
	if (com_port == COM1_BASE_ADDR)
	{
		irq_set_com = irq_set_serial;
	}
	else if (com_port == COM2_BASE_ADDR)
	{
		irq_set_com = irq_set_serial << 1;
	}
	else
	{
		printf("ser_int_handler: invalid com_port\n");
		return -1;
	}

	while (1)
	{
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{ /* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_com)
				{ /* subscribed interrupt */
					event = SER_EVT;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
		else
		{ /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		/* Now, do application dependent event handling */
		if (event == SER_EVT)
		{
			return handle_ser_event_rec(com_port);
		}
	}
	return 1;
}

int ser_int_handler_trsm(unsigned long com_port, unsigned char chr)
{
	int ipc_status;
	message msg;
	int r;
	char event = 0;
	unsigned int irq_set_com;
	if (com_port == COM1_BASE_ADDR)
	{
		irq_set_com = irq_set_com1;
	}
	else if (com_port == COM2_BASE_ADDR)
	{
		irq_set_com = irq_set_com2;
	}
	else
	{
		printf("ser_int_handler: invalid com_port\n");
		return -1;
	}

	while (1)
	{
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{ /* received notification */
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_com)
				{ /* subscribed interrupt */
					event = SER_EVT;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
		else
		{ /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		/* Now, do application dependent event handling */
		if (event == SER_EVT)
		{
			return handle_ser_event_trsm(com_port, chr);
		}
	}
	return 1;
}

unsigned char handle_ser_event_rec(unsigned long com_port)
{
	interrupt_id_t int_id;
	unsigned int i = 1;
	while (i)
	{
		int_id = find_int_id(com_port);
		switch (int_id)
		{
		case int_receiver_line_status:
			printf("receiver line status error\n");
			ser_unsubscribe_int(com_port);
			exit(1);
			break;
		case int_rec_data:
			i = 0;
			unsigned char rec_chr = 0;
			unsigned long tmp = 0;
			sys_inb(com_port + RB_REG, &tmp);
			rec_chr = (tmp & 0xFF);
			return rec_chr;
			break;
		}
	}
	return 0;
}

int handle_ser_event_trsm(unsigned long com_port, unsigned char trsm_chr)
{
	interrupt_id_t int_id;
	unsigned int i = 1;
	while (i)
	{
		int_id = find_int_id(com_port);
		switch (int_id)
		{
		case int_thr_empty:
			i = 0;
			unsigned long tmp = trsm_chr;
			sys_outb(com_port + TH_REG, tmp);
			return 0;
			break;
		}
	}
	return 1;
}

interrupt_id_t find_int_id(unsigned long com_port)
{
	unsigned long data = 0;
	sys_inb(com_port + II_REG, &data);
	if (data & NO_INT)
		return no_int;
	else
	{
		if ((data & INT_PEND) == RLS_INT_PEND)
			return int_receiver_line_status;
		if ((data & INT_PEND) == RD_INT_PEND)
			return int_rec_data;
		if ((data & INT_PEND) == CT_INT_PEND)
			return int_char_timeout;
		if ((data & INT_PEND) == THRE_INT_PEND)
			return int_thr_empty;
	}
}

int enable_ser_interrupts(unsigned long com_port, unsigned char tx)
{
	unsigned long data = 0;
	sys_inb(com_port + IE_REG, &data);
	if (tx == 0)
		data |= (EN_RDA_INT | EN_LS_INT);
	else
		data |= EN_THRE_INT;
	sys_outb(com_port + IE_REG, data);
	return 0;
}
