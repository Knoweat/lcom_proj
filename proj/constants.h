#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define TEMPORARY_BACKGROUND 0xF81F

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

#define VG_MODE 0x114

typedef enum
{
	no_serial, serial_p1, serial_p2
} game_mode_t;

#define X_INIT_BTN 337
#define X_END_BTN 493
#define Y_INIT_BTN1 324
#define Y_END_BTN1 387
#define Y_INIT_BTN2 399
#define Y_END_BTN2 462

#define CURSOR_H_DIM 30
#define CURSOR_V_DIM 48


#define SCREEN_H_DIM 800
#define SCREEN_V_DIM 600

#define DELTA_ALARM 30
#define SERIAL_ACTIVE 0xFE

#define MOVE_FRONT 0x30
#define MOVE_BACK 0x31
#define ATTACK_HIGH 0x32
#define ATTACK_MEDIUM 0x33
#define ATTACK_LOW 0x34
#define DEFENCE_HIGH 0x35
#define DEFENCE_MEDIUM 0x36
#define DEFENCE_LOW 0x37

#define ATT_CONST 20

#define GAME_TIME 60

#define KBD_Z 0xAC
#define KBD_X 0xAD
#define KBD_Y 0x95
#define KBD_U 0x96
#define KBD_H 0xA3
#define KBD_J 0xA4
#define KBD_N 0xB1
#define KBD_M 0xB2

#define P1_INIT_POS_X 200
#define P2_INIT_POS_X 460
#define P_INIT_POS_Y 350
#define P_INIT_LIFE 100
#define P1_LIFE_POS_X 100
#define P2_LIFE_POS_X 500
#define P_LIFE_POS_Y 20

#endif
