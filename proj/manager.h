#ifndef __MANAGER_H
#define __MANAGER_H

#include "sprites.h"
#include "fighter.h"

typedef enum{no_serial, serial_p1, serial_p2}game_mode_t;

int startup_game();

int finish_round(fighter *p1, fighter *p2);
#endif
