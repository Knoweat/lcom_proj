#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

#define VBE_FUNC 0x4F00
#define GET_VBE_MODE 0x01
#define BIOS_VIDEO_SERV 0x10
#define VBE_MODE_INFO_SIZE 256

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u r;

	mmap_t map;

	lm_init();
	if (lm_alloc(VBE_MODE_INFO_SIZE, &map) == NULL) {
		printf("allocation of memory error.\n");
		return -1;
	}

	r.u.w.ax = VBE_FUNC | GET_VBE_MODE;
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);
	r.u.w.cx = mode;
	r.u.b.intno = BIOS_VIDEO_SERV;
	if (sys_int86(&r) != OK) {
		printf("vbe_get_mode_info: get of video mode error.\n");
		return -1;
	}

	if (r.u.b.ah == 0x01) {
		printf("Function call failed.\n");
		return -1;
	}
	if (r.u.b.ah == 0x02) {
		printf("Function is not supported in current HW configuration.\n");
		return -1;
	}
	if (r.u.b.ah == 0x03) {
		printf("Function is invalid in current video mode.\n");
		return -1;
	}

	memcpy(vmi_p, map.virtual, VBE_MODE_INFO_SIZE);
	lm_free(&map);
	return 0;
}

