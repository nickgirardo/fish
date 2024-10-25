#include "camera.h"

#define CAMERA_STOP_LEFT 0
#define CAMERA_STOP_RIGHT 0x30

char camera_scroll;
signed char camera_req_scroll;

void init_camera() {
  camera_scroll = 0;
  camera_req_scroll = 0;
}

void update_camera() {
  unsigned char i;
  EntityData *entity;

  if (camera_scroll < CAMERA_STOP_RIGHT && camera_req_scroll > 0) {
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

  if (camera_scroll > CAMERA_STOP_LEFT && camera_req_scroll < 0) {
    camera_scroll--;

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
	if (entities[i] == EntityEmpty) break;

	entity = &entity_data[i];
	entity->x.hl.h++;
    }
  }
}
