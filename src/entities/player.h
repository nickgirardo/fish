#ifndef  PLAYER_H
#define  PLAYER_H

#include "../gt/input.h"
#include "../gt/drawing_funcs.h"
#include "../common.h"

#define PLAYER_ACCEL 22
#define PLAYER_MIN_V 0x0030
#define PLAYER_MIN_V_NEG -0x0030
#define PLAYER_FRICTION_COEFF 3
#define PLAYER_SIZE 6

void init_player(char x, char y);
void reset_player(char x, char y);
void draw_player(char ix);
void update_player(char ix);


#endif //  PLAYER_H
