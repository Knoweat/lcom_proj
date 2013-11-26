#include "game_manager.h"

#include "constants.h"
#include "cursor.xpm"

unsigned int made_exit = 1;
extern unsigned int alarm_visible;

extern fighter p1, p2;
extern sprite_t background, cursor, start_menu, alarm_message, end_image,
		ganhou_perdeu, perdeu_ganhou, ganhou, perdeu;

extern game_mode_t game_mode;

int load_game()
{
	unsigned char **sprite_f;

	p1 = *create_fighter(1, 0.4, 0.7);
	p2 = *create_fighter(2, 0.7, 0.4);

	//load player 1 sprites
	if (load_to_strings("/home/proj/sprites/p1_stand1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.waiting_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_stand2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.waiting_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_walk1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.walking_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_walk2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.walking_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_walk3.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.walking_stance[2]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_walk4.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.walking_stance[3]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_la1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.low_attack_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_la2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.low_attack_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_ma1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.medium_attack_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_ma2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.medium_attack_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_ha1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.high_attack_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_ha2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.high_attack_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_ld.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.low_defense_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_md.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.medium_defense_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_hd.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.high_defense_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_win.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.win_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_die1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.die_stance[0]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_die2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.die_stance[1]), p1.pos_x, p1.pos_y);

	if (load_to_strings("/home/proj/sprites/p1_hit.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p1.hit_stance[0]), p1.pos_x, p1.pos_y);

	//load player 2 sprites
	if (load_to_strings("/home/proj/sprites/p2_stand1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}

	load_sprite(sprite_f, &(p2.waiting_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_stand2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.waiting_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_walk1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.walking_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_walk2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.walking_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_walk3.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.walking_stance[2]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_walk4.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.walking_stance[3]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_la1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.low_attack_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_la2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.low_attack_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_ma1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.medium_attack_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_ma2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.medium_attack_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_ha1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.high_attack_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_ha2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.high_attack_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_ld.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.low_defense_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_md.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.medium_defense_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_hd.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.high_defense_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_win.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.win_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_die1.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.die_stance[0]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_die2.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.die_stance[1]), p2.pos_x, p2.pos_y);

	if (load_to_strings("/home/proj/sprites/p2_hit.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &(p2.hit_stance[0]), p2.pos_x, p2.pos_y);

	//load background
	if (load_to_strings("/home/proj/sprites/background3.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &background, 0, 0);

	if (load_to_strings("/home/proj/sprites/startmenu.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &start_menu, 0, 0);

	if (load_to_strings("/home/proj/sprites/end_image.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &end_image, 0, 0);
	if (load_to_strings("/home/proj/sprites/ganhou_perdeu.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &ganhou_perdeu, 0, 150);

	if (load_to_strings("/home/proj/sprites/perdeu_ganhou.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &perdeu_ganhou, 0, 150);

	if (load_to_strings("/home/proj/sprites/ganhou.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &ganhou, 0, 150);

	if (load_to_strings("/home/proj/sprites/perdeu.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &perdeu, 0, 150);

	load_sprite((unsigned char **) cursor_xpm, &cursor, SCREEN_H_DIM / 2,
			SCREEN_V_DIM / 2);

	if (load_to_strings("/home/proj/sprites/vai_estudar.txt", &sprite_f) != 0)
	{
		handle_exit();
	}
	load_sprite(sprite_f, &alarm_message, 0, 570);

	return 0;
}

int startup_game()
{
	init_startup_devices();
	vg_draw_background(&start_menu);
	vg_draw_sprite(&cursor);
	vg_update_display();

	handle_startup_ints();

	term_startup_devices();
	//free(cursor.ptr);
	//free(start_menu.ptr);
	return 0;
}

int update_display()
{
	vg_draw_background(&background);
	stance_t tmp = p2.stance;
	if (tmp == HA1 || tmp == HA2 || tmp == MA1 || tmp == MA2 || tmp == LA1
			|| tmp == LA2)
	{
		vg_draw_sprite(get_curr_sprite(&p1));
		vg_draw_sprite(get_curr_sprite(&p2));
	}
	else
	{
		vg_draw_sprite(get_curr_sprite(&p2));
		vg_draw_sprite(get_curr_sprite(&p1));
	}
	draw_life(P1_LIFE_POS_X, P_LIFE_POS_Y, p1.life);
	draw_life(P2_LIFE_POS_X, P_LIFE_POS_Y, p2.life);
	draw_nr(355, 10, GAME_TIME - (timer0_counter / 60));
	if (alarm_visible)
		vg_draw_sprite(&alarm_message);
	vg_update_display();
	return 0;
}

void handle_exit()
{
	//fazer free de player (sprites), background
	vg_draw_sprite(&end_image);
	vg_update_display();
	sleep(2);
	printf("fuck it\n");
	if (made_exit != 1)
		term_round_devices();
	printf("almost there\n");
	vg_exit();
	exit(0);
}
