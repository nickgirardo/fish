#include "BoxPatrol.h"

#include "../gt/drawing_funcs.h"
#include "player.h"

#pragma code-name (push, "PROG0")

void init_boxpatrol(char x, char y, char w, char h, Direction dir, char offset) {
  BoxPatrolData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityBoxPatrol;

      data = (BoxPatrolData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->w = w;
      data->h = h;
      data->dir = dir;
      data->offset = offset;

      return;
    }
  }
}

void draw_boxpatrol(char ix) {
  BoxPatrolData data;

  data = *((BoxPatrolData *) &entity_data[ix]);

  await_draw_queue();
  draw_box(data.x.hl.h, data.y.hl.h, BOXPATROL_SIZE, BOXPATROL_SIZE, BOXPATROL_COLOR);
}

void update_boxpatrol(char ix) {
  BoxPatrolData *data;

  data = (BoxPatrolData *) &entity_data[ix];

  switch (data->dir) {
    case DirDown:
      data->y.hl.h++;

      data->offset++;
      if (data->offset == data->h) {
        data->offset = 0;
        data->dir = DirRight;
      }
      break;
    case DirRight:
      data->x.hl.h++;

      data->offset++;
      if (data->offset == data->w) {
        data->offset = 0;
        data->dir = DirUp;
      }
      break;
    case DirUp:
      data->y.hl.h--;

      data->offset++;
      if (data->offset == data->h) {
        data->offset = 0;
        data->dir = DirLeft;
      }
      break;
    case DirLeft:
      data->x.hl.h--;

      data->offset++;
      if (data->offset == data->w) {
        data->offset = 0;
        data->dir = DirDown;
      }
      break;
  }
}

CollisionResult collision_boxpatrol(char ix) {
  BoxPatrolData data;

  data = *((BoxPatrolData *) &entity_data[ix]);

  if (box_collision(data.x.hl.h,
                    data.x.hl.h + BOXPATROL_SIZE,
                    data.y.hl.h,
                    data.y.hl.h + BOXPATROL_SIZE,
                    player_data->x.hl.h,
                    player_data->r,
                    player_data->y.hl.h,
                    player_data->d)) {
    return ResultFail;
  }

  return ResultOk;
}

#pragma code-name (pop)
