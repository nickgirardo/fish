#include "camera.h"

#pragma code-name (push, "PROG0")

Coord camera_x_scroll;
Coord camera_y_scroll;

void init_camera() {
  camera_x_scroll.c = 0;
  camera_y_scroll.c = 0;
}

void update_camera() {
  camera_x_scroll.c += 20;
}

#pragma code-name (pop)
