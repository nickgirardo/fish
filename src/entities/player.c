#include "player.h"

#pragma code-name (push, "PROG0")

// TODO rm, just putting these here for now
#define TILE_GOAL_SECRET 0x13
#define TILE_GOAL 0x14
#define TILE_KILL 0xBB

void init_player(char x, char y) {
  PlayerData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityPlayer;

      data = (PlayerData *) &entity_data[i];

      data->x.hl.h = x;
      data->x.hl.l = 0;
      data->y.hl.h = y;
      data->y.hl.l = 0;
      data->vx.c = 0;
      data->vy.c = 0;

      data->r = 0;
      data->d = 0;

      player_data = data;

      return;
    }
  }

  // This should never be reached!!
}

void reset_player(char x, char y) {
  player_data->x.hl.h = x;
  player_data->x.hl.l = 0;
  player_data->y.hl.h = y;
  player_data->y.hl.l = 0;

  player_data->r = x + PLAYER_SIZE;
  player_data->d = y + PLAYER_SIZE;
}

void draw_player(char ix) {
  PlayerData data;

  data = *((PlayerData *) &entity_data[ix]);

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = data.x.hl.h;
  vram[VY] = data.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = PLAYER_SIZE;
  vram[HEIGHT] = PLAYER_SIZE;
  vram[COLOR] = ~92;
  vram[START] = 1;
  wait();
}

void update_player(char ix) {
  PlayerData *data;

  data = (PlayerData *) &entity_data[ix];

  if (player1_buttons & INPUT_MASK_RIGHT) {
    data->vx.c += PLAYER_ACCEL;
  } else if (player1_buttons & INPUT_MASK_LEFT) {
    data->vx.c -= PLAYER_ACCEL;
  } else if (data->vx.c > 0 && data->vx.c < PLAYER_MIN_V) {
    data->vx.c = 0;
  } else if (data->vx.c < 0 && data->vx.c > PLAYER_MIN_V_NEG) {
    data->vx.c = 0;
  }

  if (player1_buttons & INPUT_MASK_DOWN) {
    data->vy.c += PLAYER_ACCEL;
  } else if (player1_buttons & INPUT_MASK_UP) {
    data->vy.c -= PLAYER_ACCEL;
  } else if (data->vy.c > 0 && data->vy.c < PLAYER_MIN_V) {
    data->vy.c = 0;
  } else if (data->vy.c < 0 && data->vy.c > PLAYER_MIN_V_NEG) {
    data->vy.c = 0;
  }

  data->vx.c -= data->vx.c >> PLAYER_FRICTION_COEFF;
  data->vy.c -= data->vy.c >> PLAYER_FRICTION_COEFF;

  data->x.c += data->vx.c;
  data->y.c += data->vy.c;

  // Should we scroll the camera?
  if (data->vx.c > 0 && data->x.hl.h > CAMERA_SCROLL_START_RIGHT) {
    camera_request_x_scroll = 1;
  }

  if (data->vx.c < 0 && data->x.hl.h < CAMERA_SCROLL_START_LEFT) {
    camera_request_x_scroll = -1;
  }

  // TODO do we still want to cache these values
  data->r = data->x.hl.h + PLAYER_SIZE;
  data->d = data->y.hl.h + PLAYER_SIZE;
}

#pragma code-name (pop)
