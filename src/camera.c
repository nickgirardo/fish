#include "camera.h"

char camera_scroll;
signed char camera_req_scroll;

char scrollstop_left;
char scrollstop_right;

void scroll_all_entities(char scroll) {
  EntityData *entity;
  unsigned char i;

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      if (entities[i] == EntityEmpty) return;

	entity = &entity_data[i];
	entity->x.hl.h -= scroll;
    }
}

void init_camera() {
  camera_scroll = portal_target_scroll;
  camera_req_scroll = 0;

  scrollstop_left = portal_target_scrollstop_left;
  scrollstop_right = portal_target_scrollstop_right;

  scroll_all_entities(portal_target_scroll);
}

void update_camera() {
  if (camera_scroll < scrollstop_right && camera_req_scroll > 0) {
    camera_scroll++;

    scroll_all_entities(1);
  }

  if (camera_scroll > scrollstop_left && camera_req_scroll < 0) {
    camera_scroll--;

    scroll_all_entities(-1);
  }
}
