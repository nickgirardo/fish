#include "gt/banking.h"
#include "gt/feature/text/text.h"
#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/dynawave.h"
#include "gt/music.h"

#include "common.h"
#include "util.h"

#include "camera.h"
#include "tilemap.h"

#include "tilemap.h"
#include "score_display.h"

#include "entities/player.h"
#include "entities/ring_post.h"
#include "entities/locator.h"

#include "levels/level_test.h"

#include "gen/assets/gfx.h"
#include "gen/assets/music.h"
#include "gen/assets/sfx.h"

EntityKind entities[ENTITY_TABLE_SIZE];
EntityData entity_data[ENTITY_TABLE_SIZE];

// Cache pointers to commonly used entities
EntityData *player_data;
EntityData *ring_post_data;

unsigned char current_level;

bool ring_collected;

char seconds_remaining;
char frames_in_seconds_remaining;

LevelData levels[LEVEL_COUNT];

void noop(void) {
  return;
}

void clear_entities() {
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    entities[i] = EntityEmpty;
  }
}

void init_entities(const unsigned char *data) {
  clear_entities();

  while(*data != EntityEmpty) {
    switch (*data) {
      case EntityPlayer:
        init_player(*(++data), *(++data));
        break;
      case EntityRingPost:
        init_ring_post(*(++data), *(++data));
        break;
      default:
        // We shouldn't ever hit this branch if our levels are crafted correctly
        // Just hard-lock
        while (1) {}
    }
    data++;
  }
}

void init_level() {
  LevelData l;
  l = levels[current_level];

  init_tilemap(l.tilemap);

  init_entities(l.entities);

  init_camera();
}

void init_game() {
  current_level = STARTING_LEVEL;

  init_level();
}

void (*const drawing_fns[])(char) = {
  (void (*const)(char))noop,
  draw_player,
  draw_ring_post,
};

void (*const update_fns[])(char) = {
  (void (*const)(char))noop,
  update_player,
  update_ring_post,
};

int main() {
  char i;

  init_graphics();
  load_font(0);

  load_spritesheet(&ASSET__gfx__kelp_bg_bmp, 0);

  init_dynawave();
  init_music();

  // play_song(&ASSET__music__avhg_mid, REPEAT_LOOP);

  change_rom_bank(BANK_PROG0);

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  // Level set up
  levels[0].entities = level_test_entities;
  levels[0].name = level_test_name;

  init_game();

  frames_in_seconds_remaining = 60;
  seconds_remaining = 0x20;

  // Run forever
  while (1) {
    PROFILER_START(0);
    tick_music();

    update_inputs();

    // Process timer
    frames_in_seconds_remaining--;
    if (frames_in_seconds_remaining == 0) {
      if (seconds_remaining == 0) {
	// Do something
      }

      if (seconds_remaining <= 5)
	play_sound_effect(&ASSET__sfx__time_bin, SFX_PRIORITY);
      seconds_remaining = dec_bcd_char(seconds_remaining);
      frames_in_seconds_remaining = 60;

    }

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      if (entities[i] == EntityEmpty) break;
      update_fns[entities[i]](i);
    }

    update_camera();

    update_score_display();

    draw_tilemap();

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
      if (entities[i] == EntityEmpty) break;
      drawing_fns[entities[i]](i);
    }

    draw_locator();

    await_draw_queue();
    clear_border(0xFF);
    PROFILER_END(0);

    sleep(1);
    flip_pages();
  }

  //  We should never get here!
  return 0;
}
