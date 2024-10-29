#include "ring_post.h"

#include "../gt/gametank.h"
#include "../gt/random.h"

#include "../common.h"
#include "../camera.h"

void init_ring_post(char x, char y) {
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

      data->mid_x = x + RING_POST_HALF_SIZE;

      // TODO hardcoding for now, but this should be fine
      data->x_sector = RingPostXMiddle;
      data->y_sector = RingPostYTop;

      ring_post_data = r;

      return;
    }
  }

  // This should never be reached!!
}


void move_ring_post(char ix) {
    EntityData *r;
    RingPostData *data;
    int rand;

    r = (EntityData *) &entity_data[ix];
    data = &r->data.rpd;

    rand = rnd_weak();

    if (data->x_sector != RingPostXMiddle) {
      if (rand & 1) {
	// Case where we keep y_sector the same
	// We don't need to update the value of `y_sector`

	// We've used one bit of our rand, advance it here
	rand >>= 1;
	if (rand & 1) {
	  // Case where we set x_sector to the middle sector
	  data->x_sector = RingPostXMiddle;
	} else {
	  // Case where we set x_sector to the opposite sector
	  data->x_sector = (data->x_sector == RingPostXLeft) ? RingPostXRight : RingPostXLeft;
	}
      } else {
	// Case where we flip y_sector
	data->y_sector = (data->y_sector == RingPostYTop) ? RingPostYBottom : RingPostYTop;

	rand >>= 1;
	switch (rand & 3) {
	case 0:
	case 1:
	  // Case where we set x_sector to the opposite sector
	  data->x_sector = (data->x_sector == RingPostXLeft) ? RingPostXRight : RingPostXLeft;
	  break;
	case 2:
	  // Case where we set x_sector to the middle sector
	  data->x_sector = RingPostXMiddle;
	  break;
	case 3:
	  // Case where we keep x_sector the same
	  // We don't need to update the value of `x_sector`
	  break;
	}
      }
    } else {
      // Case where the ring was in the middle sector
      // Move to one of (lr x ud) with equal probability
      data->x_sector = rand & 1;
      data->y_sector = rand & 2;
    }

    // We've used at most 2 bits of yet unaccounted for randomness
    // and at most 5 bits of total randomness
    rand >>= 2;

    // TODO expound on the constants below
    if (data->y_sector == RingPostYTop) {
      r->y.hl.h = (rand & 31) + 13;
    } else {
      r->y.hl.h = (rand & 31) + 45;
    }

    // We've 5 bits of yet unaccounted for randomness
    // and at most 10 bits of total randomness
    rand >> 5;

    switch (data->x_sector) {
    case RingPostXLeft:
      r->x.hl.h = (rand & 63) + 28;
      break;
    case RingPostXMiddle:
      r->x.hl.h = (rand & 63) + 28 + 64 + 4;
      break;
    case RingPostXRight:
      r->x.hl.h = (rand & 63) + 28 + 128 + 8;
      break;
    }

    data->mid_x = r->x.hl.h + RING_POST_HALF_SIZE;
}

// TODO for debugging
void draw_ring_post(char ix) {
  EntityData r;

  r = entity_data[ix];

  if ((signed char) (r.x.hl.h - camera_scroll) < 0)
    return;

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = r.x.hl.h - camera_scroll;
  vram[VY] = r.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = RING_POST_SIZE;
  vram[HEIGHT] = RING_POST_SIZE;
  vram[COLOR] = ~30;
  vram[START] = 1;

  wait();

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = r.x.hl.h - camera_scroll;
  vram[VY] = r.y.hl.h + RING_POST_GAP;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = RING_POST_SIZE;
  vram[HEIGHT] = RING_POST_SIZE;
  vram[COLOR] = ~30;
  vram[START] = 1;

  wait();
}

void update_ring_post(char ix) {
    EntityData *r;
    RingPostData *data;

    r = (EntityData *) &entity_data[ix];
    data = &r->data.rpd;

    if (ring_collected) {
      move_ring_post(ix);

      ring_collected = false;
    }

    data->mid_x = r->x.hl.h + RING_POST_HALF_SIZE;

}
