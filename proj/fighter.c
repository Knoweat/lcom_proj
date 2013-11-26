#include "fighter.h"

extern unsigned long long timer0_counter;

void init_fighter(fighter* f1, fighter* f2)
{
	f1->time_begin_action = 0;
	f1->movement = 0;
	f1->not_verified = 1;
	f1->pos_x = P1_INIT_POS_X;
	f1->life = P_INIT_LIFE;
	update_sprite_pos(f1);
	f1->stance = stand1;

	f2->time_begin_action = 0;
	f2->movement = 0;
	f2->not_verified = 1;
	f2->pos_x = P2_INIT_POS_X;
	f2->life = P_INIT_LIFE;
	update_sprite_pos(f2);
	f2->stance = stand1;
}

fighter* create_fighter(int number, float power, float defense)
{
	fighter* f = malloc(sizeof(fighter));

	f->power = power;
	f->defense = defense;
	f->life = P_INIT_LIFE;
	f->stance = stand1;
	f->time_begin_action = 0;
	f->movement = 0;
	f->victories = 0;
	f->not_verified = 1;

	if (number == 1) //for player 1, cria numa determinada posicao
	{
		f->player_number = 1;
		f->pos_x = P1_INIT_POS_X;
		f->pos_y = P_INIT_POS_Y;
	}

	else if (number == 2) // cria noutra
	{
		f->player_number = 2;
		f->pos_x = P2_INIT_POS_X;
		f->pos_y = P_INIT_POS_Y;
	}
	else
	{
		printf("invalid player number");
		return NULL;
	}
	return f;
}

int move_front(fighter* f_move, fighter* f)
{
	int var = 10;

	if (f_move->player_number == 1)
	{
		if (f_move->stance == stand1 || f_move->stance == stand2)
		{
			f_move->movement = 1;
			f_move->pos_x = f_move->pos_x + var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[0]),
					get_curr_sprite(f)) == 1) //enquanto houver colisao, vai decrementando var
			{
				var--;
				f_move->pos_x = f_move->pos_x + var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk1;
			f_move->time_begin_action = timer0_counter;
		}

		if (f_move->stance == walk1)
		{
			f_move->pos_x = f_move->pos_x + var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[1]),
					get_curr_sprite(f)) == 1) //enquanto houver colisao, vai decrementando var
			{
				var--;
				f_move->pos_x = f_move->pos_x + var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk2;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk2)
		{
			f_move->pos_x = f_move->pos_x + var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[2]),
					get_curr_sprite(f)) == 1)
			{
				var--;
				f_move->pos_x = f_move->pos_x + var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk3;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk3)
		{
			f_move->pos_x = f_move->pos_x + var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[3]),
					get_curr_sprite(f)) == 1)
			{
				var--;
				f_move->pos_x = f_move->pos_x + var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk4;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk4)
		{
			f_move->stance = stand1;
			f_move->time_begin_action = timer0_counter;
			f_move->movement = 0;
		}
	}
	else if (f_move->player_number == 2)
	{
		if (f_move->stance == stand1 || f_move->stance == stand2)
		{
			f_move->movement = 1;
			f_move->pos_x = f_move->pos_x - var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[0]),
					get_curr_sprite(f)) == 1) //enquanto houver colisao, vai decrementando var
			{
				var--;
				f_move->pos_x = f_move->pos_x - var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk1;
			f_move->time_begin_action = timer0_counter;
		}

		if (f_move->stance == walk1)
		{
			f_move->pos_x = f_move->pos_x - var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[0]),
					get_curr_sprite(f)) == 1) //enquanto houver colisao, vai decrementando var
			{
				var--;
				f_move->pos_x = f_move->pos_x - var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk2;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk2)
		{
			f_move->pos_x = f_move->pos_x - var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[1]),
					get_curr_sprite(f)) == 1)
			{
				var--;
				f_move->pos_x = f_move->pos_x - var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk3;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk3)
		{
			f_move->pos_x = f_move->pos_x - var;
			update_sprite_pos(f_move);
			while (check_colision(&(f_move->walking_stance[2]),
					get_curr_sprite(f)) == 1)
			{
				var--;
				f_move->pos_x = f_move->pos_x - var;
				update_sprite_pos(f_move);
			}
			f_move->stance = walk4;
			f_move->time_begin_action = timer0_counter;
		}

		else if (f_move->stance == walk4)
		{
			f_move->stance = stand1;
			f_move->time_begin_action = timer0_counter;
			f_move->movement = 0;
		}
	}
	return 0;
}

int move_back(fighter* f_move)
{
	int var = 10;

	if (f_move->player_number == 1)
	{
		if (f_move->stance == stand1 || f_move->stance == stand2)
		{
			f_move->movement = -1;
			f_move->pos_x = f_move->pos_x - var;

			if (f_move->pos_x <= 0)
			{
				f_move->pos_x = 0;
			}
			f_move->stance = walk4;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}

		if (f_move->stance == walk4)
		{
			f_move->pos_x = f_move->pos_x - var;

			if (f_move->pos_x < 0)
			{
				f_move->pos_x = 0;
			}
			f_move->stance = walk3;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk3)
		{
			f_move->pos_x = f_move->pos_x - var;

			if (f_move->pos_x < 0)
			{
				f_move->pos_x = 0;
			}
			f_move->stance = walk2;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk2)
		{
			f_move->pos_x = f_move->pos_x - var;

			if (f_move->pos_x < 0)
			{
				f_move->pos_x = 0;
			}
			f_move->stance = walk1;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk1)
		{
			f_move->pos_x = f_move->pos_x - var;

			if (f_move->pos_x < 0)
			{
				f_move->pos_x = 0;
			}
			f_move->stance = stand1;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
			f_move->movement = 0;
		}
	}
	else if (f_move->player_number == 2)
	{
		if (f_move->stance == stand1 || f_move->stance == stand2)
		{
			f_move->movement = -1;
			f_move->pos_x = f_move->pos_x + var;

			if (f_move->pos_x >= 660)
			{
				f_move->pos_x = 660;
			}
			f_move->stance = walk4;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}

		if (f_move->stance == walk4)
		{
			f_move->pos_x = f_move->pos_x + var;

			if (f_move->pos_x > 660)
			{
				f_move->pos_x = 660;
			}
			f_move->stance = walk3;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk3)
		{
			f_move->pos_x = f_move->pos_x + var;

			if (f_move->pos_x > 660)
			{
				f_move->pos_x = 660;
			}
			f_move->stance = walk2;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk2)
		{
			f_move->pos_x = f_move->pos_x + var;

			if (f_move->pos_x > 660)
			{
				f_move->pos_x = 660;
			}
			f_move->stance = walk1;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
		}
		if (f_move->stance == walk1)
		{
			f_move->pos_x = f_move->pos_x + var;

			if (f_move->pos_x > 660)
			{
				f_move->pos_x = 660;
			}
			f_move->stance = stand1;
			f_move->time_begin_action = timer0_counter;
			update_sprite_pos(f_move);
			f_move->movement = 0;
		}
	}
	return 0;
}

void update_life(fighter* f, int lost_hp)
{
	int actual_life;
	actual_life = f->life;

	int new_life = actual_life - lost_hp;

	if (new_life <= 0)
	{
		f->stance = Die1;
		f->life = 0;
	}

	else
		f->life = new_life;
}

void update_fighter(fighter* f1, fighter* f2) //f1 e o player que se deseja atualizar
{
	int i;

	i = timer0_counter - f1->time_begin_action;

	if (f1->stance == Hit)
	{
		if (i >= 25)
		{
			if (f1->life != 0)
				f1->stance = stand1;
			else
				f1->stance = Die1;

			f1->time_begin_action = timer0_counter;
		}
	}

	if (f1->stance == Die1)
	{
		if (i >= 25)
		{
			f1->stance = Die2;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == Die2)
	{
		f1->time_begin_action = timer0_counter;
	}

	else if (f1->stance == Win)
	{
		f1->time_begin_action = timer0_counter;
	}

	else if (f1->stance == walk1)
	{
		if (i >= 7)
		{
			if (f1->movement == -1)
			{
				move_back(f1);
			}
			else if (f1->movement == 1)
			{
				move_front(f1, f2);
			}
		}
	}

	else if (f1->stance == walk2)
	{
		if (i >= 7)
		{
			if (f1->movement == -1)
			{
				move_back(f1);
			}
			else if (f1->movement == 1)
			{
				move_front(f1, f2);
			}
		}
	}

	else if (f1->stance == walk3)
	{
		if (i >= 7)
		{
			if (f1->movement == -1)
			{
				move_back(f1);
			}
			else if (f1->movement == 1)
			{
				move_front(f1, f2);
			}
		}
	}

	else if (f1->stance == walk4)
	{
		if (i >= 7)
		{
			if (f1->movement == -1)
			{
				move_back(f1);
			}
			else if (f1->movement == 1)
			{
				move_front(f1, f2);
			}
		}
	}

	else if (f1->stance == stand1)
	{

		if (i >= 15)
		{
			f1->stance = stand2;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == stand2)
	{
		if (i >= 15)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == LA1)
	{
		if (i >= 7)
		{
			f1->stance = LA2;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == LA2)
	{
		if (f1->not_verified)
		{
			unsigned int lost_hp = 0;
			if (check_colision(&(f1->low_attack_stance[1]), get_curr_sprite(f2))
					== 1) // se houver colisao num ataque
			{
				//verificar se ha defesa e que tipo de defesa
				if (f2->stance == LD)
				{
					lost_hp = 0;
				}

				else if (f2->stance == MD)
				{
					lost_hp = 0.4 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else if (f2->stance == HD)
				{
					lost_hp = 0.4 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else //se nao estava a defender...
				{
					lost_hp = 0.5 * ATT_CONST * f1->power;
				}
				update_life(f2, lost_hp);
				f2->time_begin_action = timer0_counter;
				f2->stance = Hit;
			}
			f1->not_verified = 0;
		}

		if (i >= 15)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
			f1->not_verified = 1;
		}
	}
	else if (f1->stance == MA1)
	{

		if (i >= 7)
		{
			f1->stance = MA2;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == MA2)
	{
		if (f1->not_verified)
		{
			unsigned int lost_hp = 0;
			if (check_colision(&(f1->medium_attack_stance[1]),
					get_curr_sprite(f2)) == 1) // se houver colisao num ataque
			{
				//verificar se ha defesa e que tipo de defesa
				if (f2->stance == LD)
				{
					lost_hp = 0.4 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else if (f2->stance == MD)
				{
					lost_hp = 0;
				}

				else if (f2->stance == HD)
				{
					lost_hp = 0.5 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else //se nao estava a defender...
				{
					lost_hp = 0.6 * ATT_CONST * f1->power;
					f2->stance = Hit;
				}
				update_life(f2, lost_hp);
				f2->time_begin_action = timer0_counter;
			}
			f1->not_verified = 0;
		}

		if (i >= 15)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
			f1->not_verified = 1;
		}
	}

	else if (f1->stance == HA1)
	{

		if (i >= 7)
		{
			f1->stance = HA2;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == HA2)
	{
		if (f1->not_verified)
		{
			unsigned int lost_hp = 0;
			if (check_colision(&(f1->high_attack_stance[1]),
					get_curr_sprite(f2)) == 1) // se houver colisao num ataque
			{
				//verificar se ha defesa e que tipo de defesa
				if (f2->stance == LD)
				{
					lost_hp = 0.2 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else if (f2->stance == MD)
				{
					lost_hp = 0.5 * ATT_CONST * f1->power;
					lost_hp = lost_hp - (f2->defense * lost_hp);
				}

				else if (f2->stance == HD)
				{
					lost_hp = 0;
				}

				else //se nao estava a defender...
				{
					lost_hp = 0.7 * ATT_CONST * f1->power;
					f2->stance = Hit;
				}
				update_life(f2, lost_hp);
				f2->time_begin_action = timer0_counter;
			}
			f1->not_verified = 0;
		}

		if (i >= 15)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
			f1->not_verified = 1;
		}
	}

	else if (f1->stance == LD)
	{

		if (i >= 20)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == MD)
	{

		if (i >= 20)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
		}
	}

	else if (f1->stance == HD)
	{

		if (i >= 20)
		{
			f1->stance = stand1;
			f1->time_begin_action = timer0_counter;
		}
	}
}

void update_sprite_pos(fighter* f)
{
	f->waiting_stance[0].x_init_pos = f->pos_x;
	f->waiting_stance[1].x_init_pos = f->pos_x;

	f->low_attack_stance[0].x_init_pos = f->pos_x;
	f->low_attack_stance[1].x_init_pos = f->pos_x;

	f->medium_attack_stance[0].x_init_pos = f->pos_x;
	f->medium_attack_stance[1].x_init_pos = f->pos_x;

	f->high_attack_stance[0].x_init_pos = f->pos_x;
	f->high_attack_stance[1].x_init_pos = f->pos_x;

	f->low_defense_stance[0].x_init_pos = f->pos_x;

	f->medium_defense_stance[0].x_init_pos = f->pos_x;

	f->high_defense_stance[0].x_init_pos = f->pos_x;

	f->walking_stance[0].x_init_pos = f->pos_x;
	f->walking_stance[1].x_init_pos = f->pos_x;
	f->walking_stance[2].x_init_pos = f->pos_x;
	f->walking_stance[3].x_init_pos = f->pos_x;

	f->win_stance[0].x_init_pos = f->pos_x;

	f->die_stance[0].x_init_pos = f->pos_x;
	f->die_stance[1].x_init_pos = f->pos_x;

	f->hit_stance[0].x_init_pos = f->pos_x;

	f->waiting_stance[0].y_init_pos = f->pos_y;
	f->waiting_stance[1].y_init_pos = f->pos_y;

	f->low_attack_stance[0].y_init_pos = f->pos_y;
	f->low_attack_stance[1].y_init_pos = f->pos_y;

	f->medium_attack_stance[0].y_init_pos = f->pos_y;
	f->medium_attack_stance[1].y_init_pos = f->pos_y;

	f->high_attack_stance[0].y_init_pos = f->pos_y;
	f->high_attack_stance[1].y_init_pos = f->pos_y;

	f->low_defense_stance[0].y_init_pos = f->pos_y;

	f->medium_defense_stance[0].y_init_pos = f->pos_y;

	f->high_defense_stance[0].y_init_pos = f->pos_y;

	f->walking_stance[0].y_init_pos = f->pos_y;
	f->walking_stance[1].y_init_pos = f->pos_y;
	f->walking_stance[2].y_init_pos = f->pos_y;
	f->walking_stance[3].y_init_pos = f->pos_y;

	f->win_stance[0].y_init_pos = f->pos_y;

	f->die_stance[0].y_init_pos = f->pos_y;
	f->die_stance[1].y_init_pos = f->pos_y;

	f->hit_stance[0].y_init_pos = f->pos_y;
}

sprite_t *get_curr_sprite(fighter* f)
{
	sprite_t *tmp;
	if (f->stance == stand1)
		tmp = &(f->waiting_stance[0]);

	else if (f->stance == stand2)
		tmp = &(f->waiting_stance[1]);

	else if (f->stance == walk1)
		tmp = &(f->walking_stance[0]);

	else if (f->stance == walk2)
		tmp = &(f->walking_stance[1]);

	else if (f->stance == walk3)
		tmp = &(f->walking_stance[2]);

	else if (f->stance == walk4)
		tmp = &(f->walking_stance[3]);

	else if (f->stance == LA1)
		tmp = &(f->low_attack_stance[0]);

	else if (f->stance == LA2)
		tmp = &(f->low_attack_stance[1]);

	else if (f->stance == MA1)
		tmp = &(f->medium_attack_stance[0]);

	else if (f->stance == MA2)
		tmp = &(f->medium_attack_stance[1]);

	else if (f->stance == HA1)
		tmp = &(f->high_attack_stance[0]);

	else if (f->stance == HA2)
		tmp = &(f->high_attack_stance[1]);

	else if (f->stance == LD)
		tmp = &(f->low_defense_stance[0]);

	else if (f->stance == MD)
		tmp = &(f->medium_defense_stance[0]);

	else if (f->stance == HD)
		tmp = &(f->high_defense_stance[0]);

	else if (f->stance == Win)
		tmp = &(f->win_stance[0]);

	else if (f->stance == Die1)
		tmp = &(f->die_stance[0]);

	else if (f->stance == Die2)
		tmp = &(f->die_stance[1]);

	else if (f->stance == Hit)
		tmp = &(f->hit_stance[0]);

	return tmp;
}

void win_round(fighter* f)
{
	f->stance = Win;
	f->victories++;
}

void high_attack(fighter *f)
{
	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = HA1;
		f->time_begin_action = timer0_counter;
	}
}

void medium_attack(fighter *f)
{
	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = MA1;
		f->time_begin_action = timer0_counter;
	}
}

void low_attack(fighter *f)
{
	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = LA1;
		f->time_begin_action = timer0_counter;
	}
}

void high_defence(fighter *f)
{
	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = HD;
		f->time_begin_action = timer0_counter;
	}
}

void medium_defence(fighter *f)
{
	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = MD;
		f->time_begin_action = timer0_counter;
	}
}

void low_defence(fighter *f)
{

	if (f->stance == stand1 || f->stance == stand2)
	{
		f->stance = LD;
		f->time_begin_action = timer0_counter;
	}
}
