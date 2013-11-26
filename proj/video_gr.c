#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <math.h>

#include "video_gr.h"

#define VBE_FUNC 0x4F00
#define SET_VBE_MODE 0x02
#define LINEAR_MODEL_BIT 14
#define BIOS_VIDEO_SERV 0x10

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

/* Private global variables */

static unsigned char *video_mem; /* Process address to which VRAM is mapped */
static unsigned char *video_buff;

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode)
{
	struct reg86u reg;

	vg_info_gr vi_p;
	vbe_mode_info_t vmi;

	if (vbe_get_mode_info(mode, &vmi) != 0)
	{
		return NULL;
	}

	vi_p.vram_base_phys = vmi.PhysBasePtr;
	vi_p.scr_xres = vmi.XResolution;
	vi_p.scr_yres = vmi.YResolution;
	vi_p.bits_per_pixel = vmi.BitsPerPixel;
	vi_p.vram_size = vi_p.scr_xres * vi_p.scr_yres * vi_p.bits_per_pixel;

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes)(vi_p.vram_base_phys);
	mr.mr_limit = mr.mr_base + vi_p.vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vi_p.vram_size);

	if (video_mem == MAP_FAILED)
		panic("video_gr couldn't map video memory");

	vi_p.vram_base_virt = video_mem;
	h_res = vi_p.scr_xres;
	v_res = vi_p.scr_yres;
	bits_per_pixel = vi_p.bits_per_pixel;

	reg.u.w.ax = VBE_FUNC | SET_VBE_MODE;
	reg.u.w.bx = 1 << LINEAR_MODEL_BIT | mode;
	reg.u.b.intno = BIOS_VIDEO_SERV;
	if (sys_int86(&reg) != OK)
	{
		printf("vg_init: set of video mode error.\n");
		return NULL;
	}

	if (reg.u.b.ah == 0x01)
	{
		printf("Function call failed.\n");
		return NULL;
	}
	if (reg.u.b.ah == 0x02)
	{
		printf("Function is not supported in current HW configuration.\n");
		return NULL;
	}
	if (reg.u.b.ah == 0x03)
	{
		printf("Function is invalid in current video mode.\n");
		return NULL;
	}
	video_buff = malloc((bits_per_pixel / 8) * h_res * v_res);
	if (video_buff == NULL)
	{
		printf("vg_init: Error allocating memory for video buffer");
		vg_exit();
		exit(1);
	}
	return video_mem;
}

int vg_fill(unsigned long color)
{
	unsigned char *ptr = video_buff;
	unsigned int x = 0, y = 0;
	while (y < v_res)
	{
		vg_set_pixel(x, y, color);
		if (x == h_res - 1)
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
		}
	}
	return 0;
}

int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color)
{
	unsigned char *ptr = video_buff;
	ptr += (y * h_res + x) * bits_per_pixel / 8;
	unsigned int i = 0;
	for (; i < bits_per_pixel / 8; i++)
	{
		unsigned char tmp = color & 0x000000FF;
		color >>= 8;
		*ptr = tmp;
		ptr++;
	}

	return 0;
}

int vg_set_pixel_sprite(unsigned long x, unsigned long y, unsigned long color)
{
	if (color == TEMPORARY_BACKGROUND)
		return 0;
	unsigned char *ptr = video_buff;
	ptr += (y * h_res + x) * bits_per_pixel / 8;
	unsigned int i = 0;
	for (; i < bits_per_pixel / 8; i++)
	{
		unsigned char tmp = color & 0x000000FF;
		color >>= 8;
		*ptr = tmp;
		ptr++;
	}

	return 0;
}

unsigned long vg_get_pixel(unsigned long x, unsigned long y)
{
	unsigned char *ptr = video_buff;
	ptr += (y * h_res + x) * bits_per_pixel / 8;
	unsigned char tmp = 0;
	unsigned long color = 0;
	unsigned int i = 1;
	tmp = *ptr;
	unsigned int j = 0;
	color |= (tmp << (j * 8));
	for (; i < bits_per_pixel / 8; i++)
	{
		j++;
		ptr++;
		tmp = *ptr;
		color |= (tmp << (j * 8));
	}
#ifdef DEBUG
	printf("Color: 0x%X", tmp);
#endif

	return color;
}

int vg_draw_sprite(sprite_t *sprite)
{
	unsigned long x = sprite->x_init_pos;
	unsigned long y = sprite->y_init_pos;
	unsigned long h_dim = sprite->x_dim;
	unsigned long v_dim = sprite->y_dim;
	if ((h_dim + x) > h_res || (v_dim + y) > v_res)
	{
		printf("invalid dimensions\n");
		return 1;
	}
	unsigned char *ptr = sprite->ptr;
	do
	{
		unsigned long color = 0;
		color = ((*ptr) << 8);
		ptr++;
		color |= *ptr;
		ptr++;
		vg_set_pixel_sprite(x, y, color);
		if (x == (h_dim + sprite->x_init_pos) - 1)
		{
			x = sprite->x_init_pos;
			y++;
		}
		else
		{
			x++;
		}
	} while (y < (v_dim + sprite->y_init_pos));

	return 0;
}

int vg_draw_background(sprite_t * background)
{
	unsigned long x = 0;
	unsigned long y = 0;
	unsigned long h_dim = 800;
	unsigned long v_dim = 600;
	unsigned char *ptr = background->ptr;
	do
	{
		unsigned long color = 0;
		color = ((*ptr) << 8);
		ptr++;
		color |= *ptr;
		ptr++;
		vg_set_pixel(x, y, color);
		if (x == (h_dim + background->x_init_pos) - 1)
		{
			x = background->x_init_pos;
			y++;
		}
		else
		{
			x++;
		}
	} while (y < (v_dim + background->y_init_pos));
	return 0;
}

int vg_update_display()
{
	unsigned char *ptr_buff = video_buff;
	unsigned char *ptr_mem = video_mem;
	unsigned int i = 0;
	unsigned int max = (bits_per_pixel / 8) * h_res * v_res;
	while (i < max)
	{
		*ptr_mem = *ptr_buff;
		ptr_mem++;
		ptr_buff++;
		i++;
	}
	return 0;
}

int vg_exit()
{
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	free(video_buff);

	if (sys_int86(&reg86) != OK)
	{
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	}
	else
		return 0;
}
