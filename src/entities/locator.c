#include "locator.h"

#include "../gt/gametank.h"

#include "../common.h"
#include "../camera.h"

void draw_locator() {
  EntityData *ring;
  signed char diff;

  ring = ring_post_data;

  diff = (signed char) (ring->x.hl.h - camera_scroll);

  if (diff > LOCATOR_BOUND && diff < SCREEN_WIDTH - LOCATOR_BOUND)
    return;

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = (ring->x.hl.h < camera_scroll + SCREEN_WIDTH - LOCATOR_BOUND) ? LOCATOR_INSET : SCREEN_WIDTH - LOCATOR_INSET;
  vram[VY] = ring->y.hl.h + RING_CENTER_OFFSET;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = LOCATOR_SIZE;
  vram[HEIGHT] = LOCATOR_SIZE;
  vram[COLOR] = ~30;
  vram[START] = 1;
  wait();
}
