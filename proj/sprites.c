#include "sprites.h"

int load_sprite(unsigned char *sprite_f[], sprite_t * sprite,
		unsigned long x_ini, unsigned long y_ini)
{
	unsigned int v_res = 0, h_res = 0, nr_colors = 0, nr_chars = 0;

	sscanf(sprite_f[0], "%d %d %d %d", &h_res, &v_res, &nr_colors, &nr_chars);

	sprite->x_init_pos = x_ini;
	sprite->y_init_pos = y_ini;
	sprite->x_dim = h_res;
	sprite->y_dim = v_res;
	unsigned int l = 0;
	//color_t used_colors[nr_colors];
	color_t* used_colors = (color_t*) malloc(nr_colors * sizeof(color_t));

	unsigned int k = 0;
	sprite->ptr = malloc(2 * v_res * h_res);
	if (sprite->ptr == NULL)
	{
		printf("ERRO1");
		vg_exit();
		exit(0);
	}

	unsigned int i = 1;
	while (i <= nr_colors)
	{

		used_colors[l].char_seq = (unsigned char*) malloc(
				sizeof(char) * nr_chars);
		if (used_colors[l].char_seq == NULL)
		{
			printf("ERRO2");
			vg_exit();
			exit(0);
		}

		unsigned int j = 0;
		while (j < nr_chars)
		{
			used_colors[l].char_seq[j] = sprite_f[i][j];
#ifdef DEBUG
			printf("char:%c\n", used_colors[l].char_seq[j]);
#endif
			j++;
		}
		j = j + 4;
		unsigned long colors_tmp = 0, unused = 0;
		sscanf(sprite_f[i], "%lc%*[^#]#%X", &unused, &colors_tmp);

		unsigned long red = 0, green = 0, blue = 0;
		red = (RED & colors_tmp) >> 16;
		green = (GREEN & colors_tmp) >> 8;
		blue = BLUE & colors_tmp;

		convert_888_to_565(used_colors[l].col, red, green, blue);
		used_colors[l].nr_chars = nr_chars;

#ifdef DEBUG
		printf("color:%x%x\n", used_colors[l].col[0], used_colors[l].col[1]);
		tickdelay(micros_to_ticks(50000));
#endif
		l++;
		k += 2;

		i++;
	}
	unsigned a = 0;
	while (i <= (nr_colors + v_res))
	{
		unsigned int j = 0;
		while (j < (nr_chars * h_res))
		{
			unsigned int k = 0;
			unsigned char tmp_ptr[nr_chars];
			while (k < nr_chars)
			{
				tmp_ptr[k] = sprite_f[i][j + k];
				k++;
			}
			unsigned int b = 0;
			while (b < nr_colors)
			{
				unsigned int verified = 1;
				unsigned int m = 0;
				while (m < nr_chars)
				{
					if (tmp_ptr[m] == used_colors[b].char_seq[m])
					{
						verified = 0;
					}

					if (verified != 0)
						break;
					m++;
				}
				if (verified == 0)
				{
					if (a > (2 * h_res * v_res * nr_chars))
					{
						printf("accessed invalid memory position\n");
						exit(1);
					}
					sprite->ptr[a] = used_colors[b].col[0];
					a++;
					sprite->ptr[a] = used_colors[b].col[1];
					a++;
				}
				b++;
			}
			j += nr_chars;
		}
		i++;
	}
	/*unsigned int d = 0;
	 while (d < nr_chars)
	 {
	 free(used_colors[d].char_seq);
	 d++;
	 }
	 //free(sprite_f);*/
#ifdef DEBUG
	unsigned int z = 0;
	while (z < (h_res * v_res * 2))
	{
		printf("%X", sprite->ptr[z]);
		tickdelay(micros_to_ticks(50000));
		z++;
	}
#endif
	return 0;
}

int convert_888_to_565(unsigned char *ptr, unsigned long red,
		unsigned long green, unsigned long blue)
{
	unsigned char red_tmp = 0, green_tmp1 = 0, green_tmp2 = 0, blue_tmp = 0;

	red = red >> 3;
	green = green >> 2;
	blue = blue >> 3;
	ptr[0] = ((red & 0x1f) << 3) | ((green >> 3) & 0x7); //R (5 bits) +  G (upper 3 bits)
	ptr[1] = ((green & 0x7) << 5) | (blue & 0x1f); //G (lower 3 bits) + B (5 bits)
}

int load_to_strings(unsigned char *file_name, unsigned char ***sprite_f)
{
	unsigned char c;
	FILE *fs;
	unsigned char **sprite_tmp;
	unsigned int v_length = 0;
	unsigned int h_length = 0;
	fs = fopen(file_name, "r");
	if (fs == NULL)
	{
		printf("File does not exist\n");
		return 1;
	}
	else
	{
		unsigned int i = 0;
		unsigned char sprite_info[30];
		do
		{
			c = getc(fs);
			sprite_info[i] = c;
			i++;
		} while (c != '\n');
		unsigned int h_res = 0, v_res = 0, nr_colors = 0, nr_chars = 0;
		sscanf(sprite_info, "%d %d %d %d", &h_res, &v_res, &nr_colors,
				&nr_chars);
		if (8 + nr_chars > h_res * nr_chars)
			h_length = 8 + nr_chars + 5;
		else
			h_length = (h_res * nr_chars) + 5;

		fclose(fs);
		v_length = 1 + nr_colors + v_res;
		sprite_tmp = (unsigned char**) malloc(v_length * sizeof(char*));
		if (sprite_tmp == NULL)
		{
			printf("ERRO1");
			vg_exit();
			exit(0);
		}
		unsigned int a = 0;
		for (a = 0; a < v_length; a++)
		{
			sprite_tmp[a] = (unsigned char*) malloc(
					h_length * sizeof(char) * nr_chars);
			if (sprite_tmp[a] == NULL)
			{
				printf("ERRO2");
				vg_exit();
				exit(0);
			}
		}
		fs = fopen(file_name, "r");
		if (fs == NULL)
		{
			printf("error opening file\n");
			return 1;
		}
		i = 0;
		unsigned j = 0;
		unsigned char line[h_length];
		do
		{
			if (fgets(line, h_length + 4, fs) == NULL)
			{
				break;
			}
			unsigned char *tmp = (unsigned char*) strrchr(line, '\n');
			if (tmp != NULL)
			{
				*tmp = '\0';
			}

			memcpy(sprite_tmp[j], line, h_length);
			j++;
		} while (j < v_length);
	}
	fclose(fs);
	*sprite_f = sprite_tmp;

	/*#ifdef DEBUG
	 unsigned z = 0;
	 for (z = 0; z < v_length; z++)
	 {
	 printf("%s\n", (*sprite_f)[z]);
	 tickdelay(micros_to_ticks(5000));
	 }
	 #endif*/
	return 0;
}

int draw_life(int pos_x, int pos_y, int hp)
{

	int i;
	int j;

	for (i = 1; i < 2 * hp; i++)
	{ //assumir que HP esta de 0 a 100, a barra ocupa o dobro (2 pixels por ponto de vida)

		for (j = 1; j < 10; j++)
		{
			if (hp >= 0 && hp < 20) // nao deixar HP ter valores negativos
				vg_set_pixel(pos_x + i, pos_y + j, 0xF800);
			else if (hp >= 20 && hp < 80)
				vg_set_pixel(pos_x + i, pos_y + j, 0xFC00);
			else if (hp >= 80 && hp <= 100)
				vg_set_pixel(pos_x + i, pos_y + j, 0x07D0);
			else
				return -1;
		}
	}

	return 0;
}

int draw_nr(int pos_x, int pos_y, int nr)
{
	int i = 0;
	unsigned int pos = pos_x;
	unsigned int ptr[10];
	unsigned quotient = nr;
	unsigned remainder = 0;
	while (quotient != 0)
	{
		if (i >= 10)
			return 1;
		remainder = quotient % 10;
		quotient = quotient / 10;
		ptr[i] = remainder;
		i++;
	}

	i--;
	while (i >= 0)
	{
		unsigned char **n;
		sprite_t tmp;
		char path[] = "/home/proj/sprites/";
		char number[2];
		switch (ptr[i])
		{
		case 0:
			*number = '0';
			break;
		case 1:
			*number = '1';
			break;
		case 2:
			*number = '2';
			break;
		case 3:
			*number = '3';
			break;
		case 4:
			*number = '4';
			break;
		case 5:
			*number = '5';
			break;
		case 6:
			*number = '6';
			break;
		case 7:
			*number = '7';
			break;
		case 8:
			*number = '8';
			break;
		case 9:
			*number = '9';
			break;
		}
		char type[] = ".txt";
		strcat(path, number);
		strcat(path, type);
		load_to_strings(path, &n);
		load_sprite(n, &tmp, pos, pos_y);
		vg_draw_sprite(&tmp);
		pos += 30;
		i--;
	}
}

int check_colision(sprite_t* player1, sprite_t* player2)
{
	int i;
	int j;
	int k;
	int l;

	unsigned char* ptr1;
	unsigned char* ptr2;
	int x1_init;
	int x2_init;
	int x1_dim;
	int x2_dim;
	int y1_dim;

	if (player1->x_init_pos < player2->x_init_pos)
	{
		ptr1 = player1->ptr;
		ptr2 = player2->ptr;
		x1_init = player1->x_init_pos;
		x2_init = player2->x_init_pos;
		x1_dim = player1->x_dim;
		x2_dim = player2->x_dim;
		y1_dim = player1->y_dim;
	}
	else
	{
		ptr1 = player2->ptr;
		ptr2 = player1->ptr;
		x1_init = player2->x_init_pos;
		x2_init = player1->x_init_pos;
		x1_dim = player2->x_dim;
		x2_dim = player1->x_dim;
		y1_dim = player2->y_dim;
	}

	int colision_area = x1_init + x1_dim - x2_init;
	if (colision_area <= 0) //se os sprites nao se cruzam
		return 0; // retorna 0 se nao ha colisao
	else
	{
		for (i = x1_init; i < x1_init + x1_dim - colision_area; i++) //percorre os x que se cruzam do player1
		{
			ptr1 += 2;
		}

		i = 0;
		while (i < y1_dim) //percorrer a altura do sprite
		{
			j = 0;
			while (j < colision_area) //percorrer a area de colisao
			{
				unsigned long tmp1 = 0;
				tmp1 |= *ptr1;
				tmp1 <<= 8;
				ptr1++;
				tmp1 |= *ptr1;
				ptr1++;

				unsigned long tmp2 = 0;
				tmp2 |= *ptr2;
				tmp2 <<= 8;
				ptr2++;
				tmp2 |= *ptr2;
				ptr2++;

				if ((tmp1 != TEMPORARY_BACKGROUND)
						&& (tmp2 != TEMPORARY_BACKGROUND))
					return 1; //retorna 1 caso haja colisao

				j++;

			}
			for (k = 0; k < x1_dim - colision_area; k++)
			{
				ptr1 += 2; // avancar o ptr as vezes necessarias
			}
			for (l = 0; l < x2_dim - colision_area; l++)
			{
				ptr2 += 2;
			}
			i++;
		}
	}
	return 0; //retorna 0 se nao houver colisao
}
