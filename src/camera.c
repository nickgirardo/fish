#include "camera.h"

char camera_scroll;
signed char camera_req_scroll;

char scrollstop_left;
char scrollstop_right;

void init_camera(char scroll, char left_stop, char right_stop) {
  camera_scroll = scroll;
  camera_req_scroll = 0;

  scrollstop_left = left_stop;
  scrollstop_right = right_stop;
}

void update_camera() {
  unsigned char i;
  EntityData *entity;

  if (camera_scroll < scrollstop_right && camera_req_scroll > 0) {
    camera_scroll++;

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
	if (entities[i] == EntityEmpty) break;

	entity = &entity_data[i];
	entity->x.hl.h--;
    }
  }
  if (camera_req_scroll == 0xFF) {
    while(1) {}
  }

  if (camera_scroll > scrollstop_left && camera_req_scroll < 0) {
    camera_scroll--;

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
	if (entities[i] == EntityEmpty) break;

	entity = &entity_data[i];
	entity->x.hl.h++;
    }
  }
}
