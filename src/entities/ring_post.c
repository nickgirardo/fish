#include "ring_post.h"

void init_ring_post(char x, char y, char is_top_post) {
  EntityData *r;
  RingPostData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityRingPost;

      r = &entity_data[i];
      data = (RingPostData *) &r->data.pd;

      r->x.hl.h = x;
      r->x.hl.l = 0;
      r->y.hl.h = y;
      r->y.hl.l = 0;

      data->is_top_post = (bool) is_top_post;

      if (is_top_post)
	top_ring_post_data = r;

      return;
    }
  }

  // This should never be reached!!
}


void move_ring_post(char ix, char x, char y) {
    EntityData *r;
    r = (EntityData *) &entity_data[ix];

    r->x.hl.h = x;
    r->y.hl.h = y;
}

// TODO for debugging
void draw_ring_post(char ix) {
  EntityData r;

  r = entity_data[ix];

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = r.x.hl.h;
  vram[VY] = r.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = RING_POST_SIZE;
  vram[HEIGHT] = RING_POST_SIZE;
  vram[COLOR] = ~30;
  vram[START] = 1;

  wait();
}
