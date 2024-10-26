#include "townie.h"

#pragma code-name (push, "PROG0")

void init_townie(char x, char y, char action_ix) {
  EntityData *t;
  TownieData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityTownie;

      t = (EntityData *) &entity_data[i];
      data = (TownieData *) &t->data.td;

      t->x.hl.h = x;
      t->x.hl.l = 0;
      t->y.hl.h = y;
      t->y.hl.l = 0;


      t->action = entity_actions[action_ix];

      return;
    }
  }
  // This should never be reached!!
}

// TODO this will likely be useful for other entities, move it?
#define should_render(x, width) ((signed char) x + width > 0 )

void draw_townie(char ix) {
  EntityData t;
  TownieData data;

  t = entity_data[ix];

  if (!should_render(t.x.hl.h, 12))
   return;

  data = t.data.td;

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = t.x.hl.h;
  vram[VY] = t.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = 12;
  vram[HEIGHT] = 16;
  vram[COLOR] = 65;
  vram[START] = 1;
  wait();
}

void update_townie(char ix) {
}

#pragma code-name (pop)
