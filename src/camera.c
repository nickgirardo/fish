#include "camera.h"

#define CAMERA_STOP_LEFT 0
#define CAMERA_STOP_RIGHT 0x30

void init_camera() {
  camera_scroll = 0;
}

void update_camera() {
  if (camera_scroll > CAMERA_STOP_LEFT && camera_req_scroll > 0) {
    while(1) {}
  }
}
