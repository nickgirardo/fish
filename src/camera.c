#include "camera.h"

#include "common.h"

char camera_scroll;
signed char camera_req_scroll;

void init_camera() {
  camera_scroll = START_SCROLL;
  camera_req_scroll = 0;
}

void update_camera() {
  if (camera_scroll < SCROLLSTOP_RIGHT && camera_req_scroll > 0)
    camera_scroll++;

  if (camera_scroll > SCROLLSTOP_LEFT && camera_req_scroll < 0)
    camera_scroll--;
}
