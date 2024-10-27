#ifndef CAMERA_H_
#define CAMERA_H_

#include "common.h"

#define START_SCROLL 0
#define SCROLLSTOP_LEFT 0
#define SCROLLSTOP_RIGHT 0x80

void init_camera();
void update_camera();

extern char camera_scroll;
extern signed char camera_req_scroll;

extern char scrollstop_left;
extern char scrollstop_right;

#endif
