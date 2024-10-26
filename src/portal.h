#ifndef PORTAL_H_
#define PORTAL_H_

#include "common.h"

void set_portal_target(char scroll, char stop_left, char stop_right, char player_x, char player_h);

extern bool portal_active;
extern char portal_target_scroll;
extern char portal_target_scrollstop_left;
extern char portal_target_scrollstop_right;

extern char portal_target_player_x;
extern char portal_target_player_y;

#endif
