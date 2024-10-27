#ifndef PLAYER_H
#define PLAYER_H

#include "../gt/gametank.h"
#include "../gt/input.h"
#include "../gt/drawing_funcs.h"
#include "../common.h"

#include "../camera.h"

#define PLAYER_ACCEL 20
#define PLAYER_MIN_V 0x0010
#define PLAYER_MIN_V_NEG -0x0010
#define PLAYER_FRICTION_COEFF 2
#define PLAYER_SIZE 12

#define MAX_STROKE_BOOST 0x24
#define MIN_STROKE 0x10
#define STROKE_INCREMENT 0x03

// A small value to drop the player by after they've stoked upwards
#define AUTO_SINK_TIMER 0x20
#define AUTO_SINK_START 0x06
#define AUTO_SINK_STRENGTH 10

#define CAMERA_SCROLL_START_LEFT  0x24
#define CAMERA_SCROLL_START_RIGHT 0x40

void init_player();
void draw_player(char ix);
void update_player(char ix);


#endif //  PLAYER_H
