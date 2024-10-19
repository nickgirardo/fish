#include "hblockgroup.h"

#include "../gt/gametank.h"
#include "../gt/drawing_funcs.h"
#include "../gt/banking.h"
#include "player.h"

#pragma code-name (push, "PROG0")

void init_hblockgroup(char x, char y, char n, char d, char vel_hi, char vel_lo) {
  HBlockGroupData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityHBlockGroup;

      data = (HBlockGroupData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->n = n;
      data->d_total = d;
      data->d_remaining = d;
      data->vel = (vel_hi << 8) + vel_lo;

      return;
    }
  }

  // This should never be reached!!
}

#define PREP_DRAW_BLOCK(y)                    \
    do {                                      \
        *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE; \
        vram[VY] = y;                         \
        vram[GX] = 0;                         \
        vram[GY] = 0;                         \
        vram[WIDTH] = HBLOCK_SIZE;            \
        vram[HEIGHT] = HBLOCK_SIZE;           \
        vram[COLOR] = ~HBLOCK_COLOR;          \
    } while (0)

#define DRAW_BLOCK(x)                                       \
    do {                                                    \
    vram[VX] = x;                                           \
    vram[START] = 1;                                        \
} while (0)

void draw_hblockgroup(char ix) {
  HBlockGroupData data;
  char i;

  data = *((HBlockGroupData *) &entity_data[ix]);

  await_draw_queue();

  PREP_DRAW_BLOCK(data.y.hl.h);

  for (i = 0; i < data.n; i++) {
      DRAW_BLOCK(data.x.hl.h + (i * HBLOCK_OFFSET));
  }
}

CollisionResult collision_hblockgroup(char ix) {
  HBlockGroupData *data;
  char i;

  data = (HBlockGroupData *) &entity_data[ix];

  // Test for collisions with the player
  // All blocks in an hblockgroup are horizontally aligned so only one y check is needed
  if (box_collision_y(
          data->y.hl.h,
          data->y.hl.h + HBLOCK_SIZE,
          player_data->y.hl.h,
          player_data->d) &&
      box_collision_x(
          data->x.hl.h,
          (data->x.hl.h + (HBLOCK_OFFSET * (data->n - 1))) + HBLOCK_SIZE,
          player_data->x.hl.h,
          player_data->r)) {
    // TODO I think with some bit cleverness we can avoid a lot of work here
    for (i = 0; i < data->n; i++) {
        if (box_collision_x(
                data->x.hl.h + (i * HBLOCK_OFFSET),
                data->x.hl.h + (i * HBLOCK_OFFSET) + HBLOCK_SIZE,
                player_data->x.hl.h,
                player_data->r)) {
            return ResultFail;
        }
    }
  }

  return ResultOk;
}

void update_hblockgroup(char ix) {
  HBlockGroupData *data;

  data = (HBlockGroupData *) &entity_data[ix];

  if ((signed char) data->d_remaining > 0) {
      data->y.c+=data->vel;
      data->d_remaining--;
  } else if ((signed char) data->d_remaining < 0) {
      data->y.c-=data->vel;
      data->d_remaining++;
  } if (data->d_remaining == 0) {
      data->d_total = -data->d_total;
      data->d_remaining = data->d_total;
  }

}

#pragma code-name (pop)
