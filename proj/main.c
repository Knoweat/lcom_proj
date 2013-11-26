#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include "game_manager.h"
#include "sprites.h"
#include "video_gr.h"
#include "devices.h"

fighter p1, p2;
sprite_t background;
sprite_t start_menu;
game_mode_t game_mode;
sprite_t cursor;
sprite_t alarm_message;
sprite_t end_image;
sprite_t ganhou_perdeu;
sprite_t perdeu_ganhou;
sprite_t ganhou;
sprite_t perdeu;

unsigned int alarm_visible = 0;

int main()
{
	sef_startup();

/*	 endpoint_t ep;
	 char name[256];
	 int priv_f;

	 sys_whoami(&ep, name, 256, &priv_f);

	 sys_enable_iop(ep);*/

	unsigned int round_nr = 1;
	load_game();
	vg_init(VG_MODE);
	startup_game();

	while (1)
	{
		vg_fill(0x0);
		draw_nr(375, 250, round_nr);
		vg_update_display();
		sleep(2);
		update_display(&p1, &p2, &background);
		init_round_devices();
		handle_round_ints();
		term_round_devices();
		round_nr++;
		if (p1.life < p2.life)
		{
			win_round(&p2);
		}

		else if (p1.life > p2.life)
		{
			win_round(&p1);
		}

		else if (p1.life <= 0 && p2.life <= 0)
		{
			win_round(&p1);
			win_round(&p2);
		}

		else if (p1.life == p2.life) //caso o tempo acabe por exemplo
		{
			win_round(&p1);
			win_round(&p2);
		}
		update_display();
		sleep(2);
		if (p1.victories == 2 || p2.victories == 2)
		{
			sleep(2);
			//verificar o modo de jogo
			if (game_mode == no_serial)
			{
				if (p1.victories > p2.victories)
					vg_draw_sprite(&ganhou_perdeu); //mudar sprite para ecra inteiro

				else if (p1.victories < p2.victories)
					vg_draw_sprite(&perdeu_ganhou); //mudar sprite para ecra inteiro

				else if (p1.victories == p2.victories)
					vg_draw_sprite(&ganhou);

			}

			else if (game_mode == serial_p1)
			{
				if (p1.victories > p2.victories)
					vg_draw_sprite(&ganhou);

				else if (p1.victories < p2.victories)
					vg_draw_sprite(&perdeu); //mudar sprite para ecra inteiro

				else if (p1.victories == p2.victories)
					vg_draw_sprite(&ganhou);
			}

			else if (game_mode == serial_p2)
			{
				if (p2.victories > p1.victories)
					vg_draw_sprite(&ganhou);

				else if (p2.victories < p1.victories)
					vg_draw_sprite(&perdeu); //mudar sprite para ecra inteiro

				else if (p1.victories == p2.victories)
					vg_draw_sprite(&ganhou);
			}
			vg_update_display();
			sleep(2);
			break;
		}

		else
		{
			init_fighter(&p1, &p2);
		}
	}

	handle_exit();
	return 0;
}
