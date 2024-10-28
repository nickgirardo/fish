#include "player.h"

#pragma code-name (push, "PROG0")

void init_player() {
  EntityData *p;
  PlayerData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityPlayer;

      p = (EntityData *) &entity_data[i];
      data = (PlayerData *) &p->data.pd;

      p->x.hl.h = 0;
      p->x.hl.l = 0;
      p->y.hl.h = 0;
      p->y.hl.l = 0;

      data->vx.c = 0;
      data->vy.c = 0;

      data->facing = FACING_RIGHT;

      data->stroke_boost = MAX_STROKE_BOOST;
      data->auto_sink = 0;

      data->r = 0;
      data->d = 0;

      player_data = p;

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

  char i;
  signed char c;

  PROFILER_START(1);

  p = (EntityData *) &entity_data[ix];
  data = &p->data.pd;

  // Auto sink logic
  // TODO remove auto_sink if a player bonks going up
  if (data->stroke_boost < MAX_STROKE_BOOST)
    data->stroke_boost += STROKE_INCREMENT;

  if (data->auto_sink > 0) {
    if (data->auto_sink < AUTO_SINK_START && !(player1_buttons & INPUT_MASK_UP)) {
      data->vy.c += AUTO_SINK_STRENGTH << 1;
    }

    data->auto_sink--;
  }

  // Basic movement logic
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

  // Stroke (speed boost) logic
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

  // "Friction" logic
  data->vx.c -= data->vx.c >> PLAYER_FRICTION_COEFF;
  data->vy.c -= data->vy.c >> PLAYER_FRICTION_COEFF;

  // Add our calculated velocities to our position
  p->x.c += data->vx.c;
  p->y.c += data->vy.c;

  // TODO do we still want to cache these values
  data->r = p->x.hl.h + PLAYER_SIZE;
  data->d = p->y.hl.h + PLAYER_SIZE;

  // Check and handle collisions with screen borders
  if (p->x.hl.h < BORDER_LEFT_WIDTH || p->x.hl.h > (SCREEN_WIDTH - BORDER_RIGHT_WIDTH - PLAYER_SIZE)) {
    p->x.c -= data->vx.c;
    data->vx.c *= -1;
  }

  if (p->y.hl.h < BORDER_TOP_HEIGHT || p->y.hl.h > (SCREEN_HEIGHT - BORDER_BOTTOM_HEIGHT - PLAYER_SIZE)) {
    p->y.c -= data->vy.c;
    data->vy.c *= -1;
  }

  // Check and handle collisions with ring posts
  // Ring posts always have the same x value, so we can often elimate both with a single x test
  if (box_collision_x(top_ring_post_data->x.hl.h,
		      top_ring_post_data->x.hl.h + RING_POST_SIZE,
		      p->x.hl.h,
		      p->x.hl.h + PLAYER_SIZE)) {
    // First test the top post
    if (box_collision_y(top_ring_post_data->y.hl.h,
			top_ring_post_data->y.hl.h + RING_POST_SIZE,
			p->y.hl.h,
			p->y.hl.h + PLAYER_SIZE)) {
      // We have a collision! To resolve the collision bounce the player back
      // Despite everything being squares and trig being too slow, we want to emulate a circular collision

      // Where is the player wrt the colliding post in the x direction
      c = (p->x.hl.h + PLAYER_HALF_SIZE) - (top_ring_post_data->x.hl.h + RING_POST_HALF_SIZE);

      // Bounce back against the post in the x direction
      // If the player is to the left of it and moving right they should be bounced back
      // Similarly for the other direction
      // NOTE the values 7 and -7 below were found experimentally
      if ((data->vx.c < 0 && c >= 7) || (data->vx.c > 0 && c <= -7)) {
	p->x.c -= data->vx.c;
	data->vx.c *= -1;
      }

      // The same as the above but in the y direction
      c = (p->y.hl.h + PLAYER_HALF_SIZE) - (top_ring_post_data->y.hl.h + RING_POST_HALF_SIZE);

      if ((data->vy.c < 0 && c >= 7) || (data->vy.c > 0 && c <= -7)) {
	p->y.c -= data->vy.c;
	data->vy.c *= -1;
      }
    } else if (box_collision_y(top_ring_post_data->y.hl.h + RING_POST_GAP,
			top_ring_post_data->y.hl.h + RING_POST_SIZE + RING_POST_GAP,
			p->y.hl.h,
			p->y.hl.h + PLAYER_SIZE)) {
      // Colliding with the bottom post
      // Note above the `else if`. The player cannot collide with both posts at once, this should help eliminate spurious tests
      // The code which follows is largely similar to the code above, adding a `RING_POST_GAP` where necessary

      // Where is the player wrt the colliding post in the x direction
      c = (p->x.hl.h + PLAYER_HALF_SIZE) - (top_ring_post_data->x.hl.h + RING_POST_HALF_SIZE);

      // Bounce back against the post in the x direction
      // If the player is to the left of it and moving right they should be bounced back
      // Similarly for the other direction
      // NOTE the values 7 and -7 below were found experimentally
      if ((data->vx.c < 0 && c >= 7) || (data->vx.c > 0 && c <= -7)) {
	p->x.c -= data->vx.c;
	data->vx.c *= -1;
      }

      // The same as the above but in the y direction
      c = (p->y.hl.h + PLAYER_HALF_SIZE) - (top_ring_post_data->y.hl.h + RING_POST_HALF_SIZE + RING_POST_GAP);

      if ((data->vy.c < 0 && c >= 7) || (data->vy.c > 0 && c <= -7)) {
	p->y.c -= data->vy.c;
	data->vy.c *= -1;
      }
    }
  }

  // Camera scroll logic
  camera_req_scroll = 0;
  if (data->vx.c > 0 && p->x.hl.h > CAMERA_SCROLL_START_RIGHT)
    camera_req_scroll = 1;

  if (data->vx.c < 0 && p->x.hl.h < CAMERA_SCROLL_START_LEFT)
    camera_req_scroll = -1;

  PROFILER_END(1);
}

#pragma code-name (pop)
