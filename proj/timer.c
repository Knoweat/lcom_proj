
#include "timer.h"
#include "devices.h"

#define MSB(n) ((n & 0x0000FF00) >> 8 )
#define LSB(n) (n & 0x000000FF)

unsigned int counter_t = 0;
extern unsigned int hook_id_timer0;
extern unsigned int irq_set_timer0;

int timer_set_square(unsigned long timer, unsigned long freq)
{

	unsigned long tmp = TIMER_FREQ / freq;
	unsigned long msb = MSB(tmp);
	unsigned long lsb = LSB(tmp);

	if (timer == 0)
	{

		sys_outb(TIMER_CTRL,
				TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN);
		sys_outb(TIMER_0, lsb);
		sys_outb(TIMER_0, msb);

	}
	else if (timer == 1)
	{
		printf("timer_set_square: timer is not editable.\n");
		return -1;
	}
	else if (timer == 2)
	{
		sys_outb(TIMER_CTRL,
				TIMER_SEL2 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN);
		sys_outb(TIMER_2, lsb);
		sys_outb(TIMER_2, msb);
	}
	else
	{
		printf("timer_set_square: invalid timer.\n");
		return -1;
	}
	return 0;
}

int timer_subscribe_int()
{
	hook_id_timer0 = 0;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer0) != OK)
	{
		printf("timer_subscribe_int: sys_irqsetpolicy() failed\n");
		return -1;
	}
	return 0;
}

int timer_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id_timer0) != OK)
	{
		printf("timer_subscribe_int: sys_irqrmpolicy() failed\n");
		return -1;
	}
	return 0;
}

void timer_int_handler()
{

	int ipc_status;
	message msg;
	int r;
	unsigned int tmp = 0;

	while (tmp < 60)
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
				if (msg.NOTIFY_ARG & irq_set_timer0)
				{ /* subscribed interrupt */
					tmp++;
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
	}
	counter_t++;
	return;
}

int hancle_timer0_evt(unsigned long long *counter)
{
	(*counter)++;
	return 0;
}

