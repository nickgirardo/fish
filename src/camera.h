#ifndef CAMERA_H_
#define CAMERA_H_

#include "common.h"
#include "portal.h"

void init_camera();
void update_camera();

extern char camera_scroll;
extern signed char camera_req_scroll;

extern char scrollstop_left;
extern char scrollstop_right;

#endif
