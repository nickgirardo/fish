#include "gt/banking.h"
#include "gt/feature/text/text.h"
#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/dynawave.h"
#include "gt/music.h"

#include "common.h"

#include "camera.h"
#include "tilemap.h"

#include "entities/player.h"

#include "levels/level_test.h"

#include "gen/assets/music.h"
#include "gen/assets/sfx.h"

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];
unsigned char tilemap[MAX_TILEMAP_SIZE];
unsigned char tilemap_decor[64];

unsigned char current_level;

#define MAX_DEATH_FREEZE 12
unsigned char death_freeze = 0;

LevelData levels[LEVEL_COUNT];

void noop(void) {
  return;
}

CollisionResult noop_collision(void) {
  return ResultOk;
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
      default:
        // We shouldn't ever hit this branch if our levels are crafted correctly
        // Just hard-lock
        while (1) {nop10();}
    }
    data++;
  }
}

unsigned short inc_bcd(unsigned short n) {
  if ((n & 0x000F) != 0x0009) return n + 0x0001;
  if ((n & 0x00F0) != 0x0090) return (n + 0x0010) & 0xFFF0;
  if ((n & 0x0F00) != 0x0900) return (n + 0x0100) & 0xFF00;
  if ((n & 0xF000) != 0x9000) return n + 0x1000 & 0xF000;

  return 0x9999;
}

void init_level() {
  LevelData l;
  l = levels[current_level];

  init_camera();

  // TODO obv don't hardcode this
  init_tilemap(512, l.tilemap);

  init_entities(l.entities);
}

void init_game() {
  current_level = STARTING_LEVEL;

  init_level();
}

void (*const drawing_fns[])(char) = {
  (void (*const)(char))noop,
  draw_player,
};

CollisionResult (*const test_collision[])(char) = {
  (CollisionResult (*const)(char))noop_collision,
  (CollisionResult (*const)(char))noop_collision,
};

void (*const update_fns[])(char) = {
  (void (*const)(char))noop,
  update_player,
};

int main() {
  char i;

  init_graphics();
  load_font(0);

  init_dynawave();
  init_music();

  // play_song(&ASSET__music__avhg_mid, REPEAT_LOOP);

  change_rom_bank(BANK_PROG0);

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  // Level set up
  levels[0].tilemap = level_test;
  levels[0].entities = level_test_entities;
  levels[0].name = level_test_name;

  init_game();

  // TODO rm
  // Just so the flashing doesn't kill me while testing drawing code
  clear_border(0);
  await_draw_queue();
  clear_screen(0);
  await_draw_queue();
  flip_pages();
  clear_border(0);
  await_draw_queue();
  clear_screen(0);
  await_draw_queue();

  // Run forever
  while (1) {
    PROFILER_START(0);
    tick_music();

    update_inputs();

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
	update_fns[entities[i]](i);
    }

    update_camera();

    PROFILER_START(1);
    draw_tilemap();
    PROFILER_END(1);

    for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
	drawing_fns[entities[i]](i);
    }

    await_draw_queue();
    PROFILER_END(0);

    sleep(1);
    flip_pages();
  }

  //  We should never get here!
  return 0;
}
