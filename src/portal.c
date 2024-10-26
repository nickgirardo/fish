#include "portal.h"

bool portal_active;
char portal_target_scroll;
char portal_target_scrollstop_left;
char portal_target_scrollstop_right;

char portal_target_player_x;
char portal_target_player_y;

void set_portal_target(char scroll, char stop_left, char stop_right, char player_x, char player_y) {
    portal_active = true;

    portal_target_scroll = scroll;
    portal_target_scrollstop_left = stop_left;
    portal_target_scrollstop_right = stop_right;

    portal_target_player_x = player_x;
    portal_target_player_y = player_y;
}
