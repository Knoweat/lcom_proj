#include "devices.h"

#include "game_manager.h"

unsigned long long timer0_counter = 0;
unsigned long long timer_counter_aux = 0;
unsigned long long rtc_counter = 0;

extern unsigned int made_exit;

extern game_mode_t game_mode;

unsigned int valid_packets = 0;

extern unsigned int alarm_visible;

unsigned int hook_id_timer0 = 0;
unsigned int hook_id_keyboard = 1;
unsigned int hook_id_mouse = 2;
unsigned int hook_id_rtc = 3;
unsigned int hook_id_serial = 4;

unsigned int irq_set_timer0 = 1;
unsigned int irq_set_keyboard = 2;
unsigned int irq_set_mouse = 4;
unsigned int irq_set_rtc = 8;
unsigned int irq_set_serial = 16;

extern fighter p1, p2;
extern sprite_t background, cursor, start_menu, alarm_message;

int packet_nr = 0;

extern long packet[ELEMS_MOUSE_PACKET];

int (*evt_handlers_startup[5])();
int (*evt_handlers_round[5])();

int init_startup_devices() {
	packet_nr = 0;

	/*
	 * Setting the serial port.
	 */
	disable_ser_interrupts(COM1_BASE_ADDR);
	unsigned long data = ser_read_ctrl_reg(COM1_BASE_ADDR);
	data &= 0x40;
	data |= 0x80;
	data = data | EIGHT_BITS_CHAR;
	data = data | ONE_STOP_BIT;
	data = data | ODD_PARITY;
	ser_write_ctrl_reg(COM1_BASE_ADDR, data);
	ser_set_bit_rate(COM1_BASE_ADDR, SER_BIT_RATE);
	enable_ser_interrupts(COM1_BASE_ADDR, 0);
	ser_subscribe_int(COM1_BASE_ADDR);
	unsigned long unused;
	sys_inb(COM1_BASE_ADDR + RB_REG, &unused);

	set_alarm(DELTA_ALARM);
	unsigned char ints = 0 | BIT(alarm_int);
	enable_ints(ints);
	rtc_subscribe_int(0);

	mouse_subscribe_int();
	packet_nr = 0;

	if (kbc_init() != OK) {
		return -1;
	}
	flush();
	timer_subscribe_int();
	return 0;
}

int term_startup_devices() {
	long ptr[3] = { 0, 0, 0 };
	ptr[0] = DISABLE_STREAM_M;
	if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
		return 1;

	if (mouse_unsubscribe_int() != OK) {
		return 1;
	}

	flush();

	unsigned char ints = 0 | BIT(alarm_int);
	disable_ints(ints);
	rtc_unsubscribe_int(0);

	disable_ser_interrupts(COM1_BASE_ADDR);
	ser_unsubscribe_int(COM1_BASE_ADDR);
	timer_unsubscribe_int();
	return 0;
}

int handle_startup_ints() {
	evt_handlers_startup[mouse_evt] = handle_startup_mouse_evt;
	evt_handlers_startup[rtc_evt] = handle_startup_rtc_evt;
	evt_handlers_round[timer0_evt] = handle_round_timer0_evt;
	evt_handlers_startup[serial_evt] = handle_startup_serial_evt;
	int ipc_status;
	message msg;
	int r;
	timer0_counter = 0;
	flush();

	timer_counter_aux = timer0_counter;

	while (1) {
		char event = 0;
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
					event |= BIT(mouse_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_rtc) { /* subscribed interrupt */
					event |= BIT(rtc_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_timer0) { /* subscribed interrupt */
					event |= BIT(timer0_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_serial) { /* subscribed interrupt */
					event |= BIT(serial_evt);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
		/* Now, do application dependent event handling */
		int exit = 0;
		if (event & BIT(mouse_evt))
			exit = (*evt_handlers_startup[mouse_evt])();
		if (event & BIT(rtc_evt))
			(*evt_handlers_startup[rtc_evt])();
		if (event & BIT(serial_evt))
			exit = (*evt_handlers_startup[serial_evt])();
		if (event & BIT(timer0_evt))
			exit = (*evt_handlers_round[timer0_evt])();

		if (timer0_counter - timer_counter_aux >= 1) {
			vg_draw_background(&start_menu);
			if (alarm_visible)
				vg_draw_sprite(&alarm_message);
			vg_draw_sprite(&cursor);
			vg_update_display();

			timer_counter_aux = timer0_counter;
		}
		if (exit)
			return 0; //only returns 1 when there is a reception of a choice from other pc or a selection on this pc
	}
	return -1;
}

int handle_startup_mouse_evt() {
	receive_mouse_packet();
	if (valid_packets) {
		packet_nr = 0;
		int x = (signed int) (cursor.x_init_pos + (signed char) packet[1]);
		int y = (signed int) (cursor.y_init_pos - (signed char) packet[2]);

		if (x <= 0)
			x = 0;
		if (x >= SCREEN_H_DIM - CURSOR_H_DIM)
			x = SCREEN_H_DIM - CURSOR_H_DIM;
		if (y <= 0)
			y = 0;
		if (y >= SCREEN_V_DIM - CURSOR_V_DIM)
			y = SCREEN_V_DIM - CURSOR_V_DIM;

		cursor.x_init_pos = x;
		cursor.y_init_pos = y;
		unsigned int k = pressed_button(packet[0]);

		if (x > X_INIT_BTN && x < X_END_BTN && k == 1) {
			if (y > Y_INIT_BTN1 && y < Y_END_BTN1) {
				game_mode = no_serial;
				return 1;
			} else if (y > Y_INIT_BTN2 && y < Y_END_BTN2) {
				game_mode = serial_p1;
				ser_write_char_poll(COM1_BASE_ADDR, SERIAL_ACTIVE);
				return 1;
			}
		}
		valid_packets = 0;
	}
	return 0;
}

int handle_startup_rtc_evt() {
	if (handle_rtc_event() == BIT(alarm_int))
		alarm_visible = 1;

	unsigned char ints = 0 | BIT(alarm_int);
	disable_ints(ints);
	return 0;
}

int handle_startup_serial_evt() {
	unsigned char rec_chr = 0;
	rec_chr = handle_ser_event_rec(COM1_BASE_ADDR);
	if (rec_chr == SERIAL_ACTIVE) {
		game_mode = serial_p2;
		return 1;
	} else
		return 0;
}

int init_round_devices() {
	made_exit = 0;
	speaker_ctrl(0);

	packet_nr = 0;
	timer_set_square(0, 60);

	if (game_mode != no_serial) {
		disable_ser_interrupts(COM1_BASE_ADDR);
		unsigned long data = ser_read_ctrl_reg(COM1_BASE_ADDR);
		data &= 0x40;
		data |= 0x80;
		data = data | EIGHT_BITS_CHAR;
		data = data | ONE_STOP_BIT;
		data = data | ODD_PARITY;
		ser_write_ctrl_reg(COM1_BASE_ADDR, data);
		ser_set_bit_rate(COM1_BASE_ADDR, SER_BIT_RATE);
		enable_ser_interrupts(COM1_BASE_ADDR, 0);
		ser_subscribe_int(COM1_BASE_ADDR);
		unsigned long unused;
		sys_inb(COM1_BASE_ADDR + RB_REG, &unused);
	}
	set_alarm(DELTA_ALARM);
	rtc_enable_square_wave();
	unsigned char ints = (0 | BIT(alarm_int) | BIT(periodic_int));
	enable_ints(ints);

	mouse_subscribe_int();
	keyboard_subscribe_int();
	timer_subscribe_int();
	rtc_subscribe_int(0);

	if (kbc_init() != OK) {
		return -1;
	}

	flush();
	return 0;
}

int term_round_devices() {
	made_exit = 1;
	speaker_ctrl(0);

	long ptr[3] = { 0, 0, 0 };
	ptr[0] = DISABLE_STREAM_M;
	if (issue_kbc_command(WRITE_B, 1, ptr) != OK)
		return 1;

	if (mouse_unsubscribe_int() != OK) {
		return 1;
	}
	keyboard_unsubscribe_int();

	flush();

	unsigned char ints = (0 | BIT(alarm_int) | BIT(periodic_int));
	disable_ints(ints);
	rtc_unsubscribe_int();

	if (game_mode != no_serial) {
		disable_ser_interrupts(COM1_BASE_ADDR);
		ser_unsubscribe_int(COM1_BASE_ADDR);
	}
	timer_unsubscribe_int();
	return 0;
}

int handle_round_ints() {
	evt_handlers_round[timer0_evt] = handle_round_timer0_evt;
	evt_handlers_round[keyboard_evt] = handle_round_keyboard_evt;
	evt_handlers_round[mouse_evt] = handle_round_mouse_evt;
	evt_handlers_round[rtc_evt] = handle_round_rtc_evt;
	evt_handlers_round[serial_evt] = handle_round_serial_evt;

	timer0_counter = 0;
	rtc_counter = 0;
	int ipc_status;
	message msg;
	int r;
	flush();
	play_music();
	timer_counter_aux = timer0_counter;
	while (1) {
		event_t event = 0;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) { /* subscribed interrupt */
					event |= BIT(mouse_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard) { /* subscribed interrupt */
					event |= BIT(keyboard_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_rtc) { /* subscribed interrupt */
					event |= BIT(rtc_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_timer0) { /* subscribed interrupt */
					event |= BIT(timer0_evt);
				}
				if (msg.NOTIFY_ARG & irq_set_serial) { /* subscribed interrupt */
					event |= BIT(serial_evt);
				}
			}
		} else {
		}
		int exit = 0;
		if (event & BIT(keyboard_evt))
			(*evt_handlers_round[keyboard_evt])();
		if (event & BIT(mouse_evt))
			(*evt_handlers_round[mouse_evt])();
		if (event & BIT(timer0_evt))
			exit = (*evt_handlers_round[timer0_evt])();
		if (event & BIT(rtc_evt))
			(*evt_handlers_round[rtc_evt])();
		if (event & BIT(serial_evt))
			(*evt_handlers_round[serial_evt])();

		if (timer0_counter - timer_counter_aux >= 1) {
			update_display();
			timer_counter_aux = timer0_counter;
		}
		play_music();
		update_fighter(&p1, &p2);
		update_fighter(&p2, &p1);
		if ((p1.life <= 0 && p1.stance == Die2)
				|| (p2.life <= 0 && p2.stance == Die2))
			exit = 1;
		if (exit)
			return 0;
	}
	return 1;
}

int handle_round_timer0_evt() {
	timer0_counter++;
	if (timer0_counter == GAME_TIME * 60)
		return 1;
	return 0;
}

int handle_round_keyboard_evt() {
	long data = read_scan_code();
	if (game_mode == no_serial) {
		if (data == KBD_Z)
			move_back(&p1);
		else if (data == KBD_X)
			move_front(&p1, &p2);
		else if (data == KBD_Y)
			high_defence(&p1);
		else if (data == KBD_U)
			high_attack(&p1);
		else if (data == KBD_H)
			medium_defence(&p1);
		else if (data == KBD_J)
			medium_attack(&p1);
		else if (data == KBD_N)
			low_defence(&p1);
		else if (data == KBD_M)
			low_attack(&p1);
		else if (data == 0x81)
			handle_exit();
	} else if (game_mode == serial_p1) {
		if (data == KBD_Z) {
			move_back(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, MOVE_BACK);
		} else if (data == KBD_X) {
			move_front(&p1, &p2);
			ser_write_char_poll(COM1_BASE_ADDR, MOVE_FRONT);
		} else if (data == KBD_Y) {
			high_defence(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_HIGH);
		} else if (data == KBD_U) {
			high_attack(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, ATTACK_HIGH);
		} else if (data == KBD_H) {
			medium_defence(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_MEDIUM);
		} else if (data == KBD_J) {
			medium_attack(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, ATTACK_MEDIUM);
		} else if (data == KBD_N) {
			low_defence(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_LOW);
		} else if (data == KBD_M) {
			low_attack(&p1);
			ser_write_char_poll(COM1_BASE_ADDR, ATTACK_LOW);
		}
	}
	return 0;
}

int handle_round_mouse_evt() {
	long data;
	sys_inb(DATA_REG, &data);
	packet[packet_nr] = data;

	if (packet_nr == 0) {
		if (packet[packet_nr] & BIT(3))
			packet_nr++;
	} else if (packet_nr == 1) {
		packet_nr++;
	} else if (packet_nr == 2) {
		packet_nr = 0;
		int x = (signed char) packet[1];
		int y = (signed char) packet[2];
		if (game_mode == no_serial) {
			if (packet[0] & RDOWN)
				move_back(&p2);
			else if (packet[0] & LDOWN)
				move_front(&p2, &p1);
			else if (x < -2 && y > 2)
				high_attack(&p2);
			else if (x < -2 && y >= -4 && y <= 4)
				medium_attack(&p2);
			else if (x < -2 && y < -4)
				low_attack(&p2);
			else if (x > 2 && y > 4)
				high_defence(&p2);
			else if (x > 2 && y >= -4 && y <= 4)
				medium_defence(&p2);
			else if (x > 2 && y < -2)
				low_defence(&p2);
		} else if (game_mode == serial_p2) {
			if (packet[0] & BIT(1)) {
				move_back(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, MOVE_BACK);
			} else if (packet[0] & BIT(0)) {
				move_front(&p2, &p1);
				ser_write_char_poll(COM1_BASE_ADDR, MOVE_FRONT);
			} else if (x < -2 && y > 4) {
				high_attack(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, ATTACK_HIGH);
			} else if (x < -2 && y >= -4 && y <= 4) {
				medium_attack(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, ATTACK_MEDIUM);
			} else if (x < -2 && y < -4) {
				low_attack(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, ATTACK_LOW);
			} else if (x > 2 && y > 4) {
				high_defence(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_HIGH);
			} else if (x > 2 && y >= -4 && y <= 4) {
				medium_defence(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_MEDIUM);
			} else if (x > 2 && y < -4) {
				low_defence(&p2);
				ser_write_char_poll(COM1_BASE_ADDR, DEFENCE_LOW);
			}
		}
	}
	return 0;
}

int handle_round_rtc_evt() {
	unsigned long data = handle_rtc_event();
	if (data & BIT(alarm_int)) {
		alarm_visible = 1;
		unsigned char ints = 0 | BIT(alarm_int);
		disable_ints(ints);
	}
	if (data & BIT(periodic_int)) {
		rtc_counter++;
	}
	return 0;
}

int handle_round_serial_evt() {
	unsigned char rec_chr = 0;
	rec_chr = handle_ser_event_rec(COM1_BASE_ADDR);
	if (game_mode == serial_p2) {
		if (rec_chr == MOVE_FRONT)
			move_front(&p1, &p2);
		if (rec_chr == MOVE_BACK)
			move_back(&p1);
		if (rec_chr == ATTACK_HIGH)
			high_attack(&p1);
		if (rec_chr == ATTACK_MEDIUM)
			medium_attack(&p1);
		if (rec_chr == ATTACK_LOW)
			low_attack(&p1);
		if (rec_chr == DEFENCE_HIGH)
			high_defence(&p1);
		if (rec_chr == DEFENCE_MEDIUM)
			medium_defence(&p1);
		if (rec_chr == DEFENCE_LOW)
			low_defence(&p1);
	}
	if (game_mode == serial_p1) {
		if (rec_chr == MOVE_FRONT)
			move_front(&p2, &p1);
		if (rec_chr == MOVE_BACK)
			move_back(&p2);
		if (rec_chr == ATTACK_HIGH)
			high_attack(&p2);
		if (rec_chr == ATTACK_MEDIUM)
			medium_attack(&p2);
		if (rec_chr == ATTACK_LOW)
			low_attack(&p2);
		if (rec_chr == DEFENCE_HIGH)
			high_defence(&p2);
		if (rec_chr == DEFENCE_MEDIUM)
			medium_defence(&p2);
		if (rec_chr == DEFENCE_LOW)
			low_defence(&p2);
	}
	return 0;
}
