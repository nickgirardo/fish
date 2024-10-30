#include "player.h"

#include "../gt/gametank.h"
#include "../gt/input.h"
#include "../gt/drawing_funcs.h"
#include "../gt/music.h"

#include "../common.h"

#include "../camera.h"
#include "../util.h"

#include "ring_post.h"

#include "../gen/assets/sfx.h"

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

      data->r = x + PLAYER_SIZE;
      data->d = y + PLAYER_SIZE;
      data->mid_x = x + PLAYER_HALF_SIZE;
      data->ss_x = x - camera_scroll;

      data->is_left_of_ring = true;

      data->score = 0;

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
  vram[VX] = p.x.hl.h - camera_scroll;
  vram[VY] = p.y.hl.h;
  vram[GX] = 0;
  vram[GY] = 0;
  vram[WIDTH] = PLAYER_SIZE;
  vram[HEIGHT] = PLAYER_SIZE;
  vram[COLOR] = ~92;
  vram[START] = 1;
  wait();
}

#define play_sfx_if_BONK()						\
  do {									\
  if (!(data->vx.c < 0x0040 && data->vx.c > -0x0040) || !(data->vx.c < 0x0040 && data->vx.c > -0x0040)) \
    play_sound_effect(&ASSET__sfx__bonk_bin, SFX_PRIORITY);		\
  } while(0);

void update_player(char ix) {
  EntityData *p;
  PlayerData *data;

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

  // Cache common calculated values
  data->r = p->x.hl.h + PLAYER_SIZE;
  data->d = p->y.hl.h + PLAYER_SIZE;
  data->mid_x = p->x.hl.h + PLAYER_HALF_SIZE;
  data->ss_x = p->x.hl.h - camera_scroll;

  // Check and handle collisions with screen borders
  if (p->x.hl.h < BORDER_LEFT_WIDTH || data->ss_x >= (SCREEN_WIDTH - BORDER_RIGHT_WIDTH - PLAYER_SIZE)) {
    play_sfx_if_BONK();
    p->x.c -= data->vx.c;
    data->vx.c *= -1;
  }

  if (p->y.hl.h < BORDER_TOP_HEIGHT || p->y.hl.h > (SCREEN_HEIGHT - BORDER_BOTTOM_HEIGHT - PLAYER_SIZE)) {
    play_sfx_if_BONK();
    p->y.c -= data->vy.c;
    data->vy.c *= -1;
  }

  // Check and handle collisions with ring posts
  // Ring posts always have the same x value, so we can often elimate both with a single x test
  if (box_collision_x(ring_post_data->x.hl.h,
		      ring_post_data->x.hl.h + RING_POST_SIZE,
		      p->x.hl.h,
		      data->r)) {
    // First test the top post
    if (box_collision_y(ring_post_data->y.hl.h,
			ring_post_data->y.hl.h + RING_POST_SIZE,
			p->y.hl.h,
		    data->d)) {
      // We have a collision!
      play_sfx_if_BONK();

      // To resolve the collision bounce the player back
      // Despite everything being squares and trig being too slow, we want to emulate a circular collision

      // Where is the player wrt the colliding post in the x direction
      c = (p->x.hl.h + PLAYER_HALF_SIZE) - (ring_post_data->data.rpd.mid_x);

      // Bounce back against the post in the x direction
      // If the player is to the left of it and moving right they should be bounced back
      // Similarly for the other direction
      // NOTE the values 7 and -7 below were found experimentally
      if ((data->vx.c < 0 && c >= 7) || (data->vx.c > 0 && c <= -7)) {
	p->x.c -= data->vx.c;
	data->vx.c *= -1;
      }

      // The same as the above but in the y direction
      c = (p->y.hl.h + PLAYER_HALF_SIZE) - (ring_post_data->y.hl.h + RING_POST_HALF_SIZE);

      if ((data->vy.c < 0 && c >= 7) || (data->vy.c > 0 && c <= -7)) {
	p->y.c -= data->vy.c;
	data->vy.c *= -1;
      }
    } else if (box_collision_y(ring_post_data->y.hl.h + RING_POST_GAP,
			ring_post_data->y.hl.h + RING_POST_SIZE + RING_POST_GAP,
			p->y.hl.h,
			data->d)) {
      // Colliding with the bottom post
      play_sfx_if_BONK();

      // Note above the `else if`. The player cannot collide with both posts at once, this should help eliminate spurious tests
      // The code which follows is largely similar to the code above, adding a `RING_POST_GAP` where necessary

      // Where is the player wrt the colliding post in the x direction
      c = data->mid_x - (ring_post_data->data.rpd.mid_x);

      // Bounce back against the post in the x direction
      // If the player is to the left of it and moving right they should be bounced back
      // Similarly for the other direction
      // NOTE the values 7 and -7 below were found experimentally
      if ((data->vx.c < 0 && c >= 7) || (data->vx.c > 0 && c <= -7)) {
	p->x.c -= data->vx.c;
	data->vx.c *= -1;
      }

      // The same as the above but in the y direction
      c = (p->y.hl.h + PLAYER_HALF_SIZE) - (ring_post_data->y.hl.h + RING_POST_HALF_SIZE + RING_POST_GAP);

      if ((data->vy.c < 0 && c >= 7) || (data->vy.c > 0 && c <= -7)) {
	p->y.c -= data->vy.c;
	data->vy.c *= -1;
      }
    }
  }

  // Check if the player has crossed the ring
  // First we must confirm the player is in the correct y position
  // TODO for now we're using a full check even though it shouldn't be necessary
  if (box_collision(ring_post_data->x.hl.h,
		    ring_post_data->x.hl.h + RING_POST_SIZE,
		    ring_post_data->y.hl.h + RING_POST_SIZE,
		    ring_post_data->y.hl.h + RING_POST_GAP,
		    p->x.hl.h,
		    data->r,
		    p->y.hl.h,
		    data->d)) {
    // First clause checks if the player is crossing the ring from the left
    // Second checks if the player crossing the ring from the right
    if (((data->mid_x > ring_post_data->data.rpd.mid_x) && data->is_left_of_ring) ||
	((data->mid_x <= ring_post_data->data.rpd.mid_x) && !data->is_left_of_ring)) {
      // Increment score
      data->score = inc_bcd_char(data->score);

      play_sound_effect(&ASSET__sfx__ring_bin, SFX_PRIORITY);
      // Mark the ring as collected by this entity
      ring_collected = ix;
    }
  }

  // Update whether the player was to the left or right of the ring
  // It's important that this follows the crossed ring check!
  data->is_left_of_ring = (data->mid_x <= ring_post_data->data.rpd.mid_x);

  // Camera scroll logic
  camera_req_scroll = 0;
  if (data->vx.c > 0 && data->ss_x > CAMERA_SCROLL_START_RIGHT)
    camera_req_scroll = 1;

  if (data->vx.c < 0 && data->ss_x < CAMERA_SCROLL_START_LEFT)
    camera_req_scroll = -1;

  PROFILER_END(1);
}

#pragma code-name (pop)
