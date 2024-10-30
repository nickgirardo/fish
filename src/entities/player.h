#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_ACCEL 20
#define PLAYER_MIN_V 0x0010
#define PLAYER_MIN_V_NEG -0x0010
#define PLAYER_FRICTION_COEFF 2

#define PLAYER_SIZE 12
#define PLAYER_HALF_SIZE 6

// The absolute speed at which a player must hit an object to play a bonk sfx
#define BONK_VEL 0x0040

#define MAX_STROKE_BOOST 0x24
#define MIN_STROKE 0x10
#define STROKE_INCREMENT 0x03

// A small value to drop the player by after they've stoked upwards
#define AUTO_SINK_TIMER 0x20
#define AUTO_SINK_START 0x06
#define AUTO_SINK_STRENGTH 10

#define CAMERA_SCROLL_START_LEFT  0x24
#define CAMERA_SCROLL_START_RIGHT 0x40

void init_player(char x, char y);
void draw_player(char ix);
void update_player(char ix);


#endif //  PLAYER_H
