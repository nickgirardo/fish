#include "camera.h"

char camera_scroll;
signed char camera_req_scroll;

void scroll_all_entities(char scroll) {
  EntityData *entity;
  unsigned char i;

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      if (entities[i] == EntityEmpty) break;

	entity = &entity_data[i];
	entity->x.hl.h -= scroll;
    }
}

void init_camera() {
  camera_scroll = START_SCROLL;
  camera_req_scroll = 0;
}

void update_camera() {
  if (camera_scroll < SCROLLSTOP_RIGHT && camera_req_scroll > 0) {
    camera_scroll++;

    scroll_all_entities(1);
  }

  if (camera_scroll > SCROLLSTOP_LEFT && camera_req_scroll < 0) {
    camera_scroll--;

    scroll_all_entities(-1);
  }
}
