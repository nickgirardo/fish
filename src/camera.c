#include "camera.h"

#pragma code-name (push, "PROG0")

#define CAMERA_STOP_LEFT 0
#define CAMERA_STOP_RIGHT 0x30

void init_camera() {
  camera_x_scroll = 0;
}

void update_camera() {
  if (camera_x_scroll > CAMERA_STOP_LEFT && camera_request_x_scroll > 0) {
    while(1) {}
  }
}

#pragma code-name (pop)
