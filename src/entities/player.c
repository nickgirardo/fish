#include "player.h"

#pragma code-name (push, "PROG0")

void init_player(char x, char y) {
  EntityData *p;
  PlayerData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityPlayer;

      p = (EntityData *) &entity_data[i];
      data = (PlayerData *) &p->data.pd;

      p->x.hl.h = x;
      p->x.hl.l = 0;
      p->y.hl.h = y;
      p->y.hl.l = 0;

      data->vx.c = 0;
      data->vy.c = 0;

      data->facing = FACING_RIGHT;

      data->stroke_boost = MAX_STROKE_BOOST;
      data->auto_sink = 0;

      data->r = 0;
      data->d = 0;

      player_data = data;

      return;
    }
  }

  // This should never be reached!!
}

void draw_player(char ix) {
  EntityData p;

  p = entity_data[ix];

  *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
  vram[VX] = p.x.hl.h;
  vram[VY] = p.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = PLAYER_SIZE;
  vram[HEIGHT] = PLAYER_SIZE;
  vram[COLOR] = ~92;
  vram[START] = 1;
  wait();
}

void update_player(char ix) {
  EntityData *p;
  PlayerData *data;

  p = (EntityData *) &entity_data[ix];
  data = &p->data.pd;

  if (data->stroke_boost < MAX_STROKE_BOOST)
    data->stroke_boost += STROKE_INCREMENT;

  if (data->auto_sink > 0) {
    if (data->auto_sink < AUTO_SINK_START && !(player1_buttons & INPUT_MASK_UP)) {
      data->vy.c += AUTO_SINK_STRENGTH << 1;
    }

    data->auto_sink--;
  }


  if (player1_buttons & INPUT_MASK_RIGHT) {
    data->facing = FACING_RIGHT;
    data->vx.c += PLAYER_ACCEL;
  } else if (player1_buttons & INPUT_MASK_LEFT) {
    data->facing = FACING_LEFT;
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

  if (data->stroke_boost > MIN_STROKE && player1_new_buttons & INPUT_MASK_B) {
    if (player1_buttons & INPUT_MASK_RIGHT) {
      if (player1_buttons & INPUT_MASK_DOWN) {
	data->vx.c += ((short) data->stroke_boost) << 4;
	data->vy.c += ((short) data->stroke_boost);
      } else {
	data->vx.c += ((short) data->stroke_boost) << 4;
	data->vy.c -= ((short) data->stroke_boost) << 2;
	data->auto_sink = AUTO_SINK_TIMER;
      }
    } else if (player1_buttons & INPUT_MASK_LEFT) {
      if (player1_buttons & INPUT_MASK_DOWN) {
	data->vx.c -= ((short) data->stroke_boost) << 4;
	data->vy.c += ((short) data->stroke_boost);
      } else {
	data->vx.c -= ((short) data->stroke_boost) << 4;
	data->vy.c -= ((short) data->stroke_boost) << 2;
	data->auto_sink = AUTO_SINK_TIMER;
      }
    } else {
      if (data->facing == FACING_RIGHT) {
	data->vx.c += ((short) data->stroke_boost) << 2;
      } else {
	data->vx.c -= ((short) data->stroke_boost) << 2;
      }

      if (player1_buttons & INPUT_MASK_DOWN) {
	data->vy.c += ((short) data->stroke_boost) << 3;
      } else {
	data->vy.c -= ((short) data->stroke_boost) << 4;
	data->auto_sink = AUTO_SINK_TIMER;
      }
    }

    data->stroke_boost = 0;
  }

  data->vx.c -= data->vx.c >> PLAYER_FRICTION_COEFF;
  data->vy.c -= data->vy.c >> PLAYER_FRICTION_COEFF;

  p->x.c += data->vx.c;
  p->y.c += data->vy.c;

  camera_req_scroll = 0;
  // Should we scroll the camera?
  if (data->vx.c > 0 && p->x.hl.h > CAMERA_SCROLL_START_RIGHT) {
    camera_req_scroll = 1;
  }

  if (data->vx.c < 0 && p->x.hl.h < CAMERA_SCROLL_START_LEFT) {
    camera_req_scroll = -1;
  }

  // TODO do we still want to cache these values
  data->r = p->x.hl.h + PLAYER_SIZE;
  data->d = p->y.hl.h + PLAYER_SIZE;
}

#pragma code-name (pop)
