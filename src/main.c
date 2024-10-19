#include "entities/score_entry.h"
#include "entities/secret_reward.h"
#include "gt/banking.h"
#include "gt/feature/text/text.h"
#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/dynawave.h"
#include "gt/music.h"

#include "common.h"

#include "credits.h"

#include "scores.h"
#include "tilemap.h"
#include "entities/player.h"
#include "entities/hblockgroup.h"
#include "entities/loopboy.h"
#include "entities/BoxPatrol.h"
#include "entities/secret.h"
#include "entities/menu.h"
#include "entities/secret_reward.h"

#include "levels/level_menu.h"
#include "levels/level_one.h"
#include "levels/level_two.h"
#include "levels/level_three.h"
#include "levels/level_four.h"
#include "levels/level_gg.h"
#include "levels/level_scores.h"

#include "gen/assets/music.h"
#include "gen/assets/sfx.h"

EntityKind entities[ENTITY_TABLE_SIZE];
PlayerData *player_data;
EntityData entity_data[ENTITY_TABLE_SIZE];
unsigned char tilemap[TILEMAP_SIZE];
unsigned char tilemap_decor[64];

MajorMode major_mode;
unsigned char current_level;
unsigned char secrets_collected;
unsigned short fail_count;

unsigned char needs_draw_fail_count;
unsigned char needs_draw_full_level;

ScoreEntry normal_scores[SCORE_ENTRIES];
ScoreEntry secret_scores[SCORE_ENTRIES];

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
      case EntityHBlockGroup:
        init_hblockgroup(*(++data), *(++data), *(++data), *(++data), *(++data), *(++data));
        break;
      case EntityLoopBoy:
        init_loopboy(*(++data), *(++data), *(++data), *(++data));
        break;
      case EntityBoxPatrol:
        init_boxpatrol(*(++data), *(++data), *(++data), *(++data), *(++data), *(++data));
        break;
      case EntitySecret:
        init_secret(*(++data), *(++data));
        break;
      case EntityMenu:
        init_menu();
        break;
      case EntitySecretReward:
        if (secrets_collected == MAX_SECRETS)
          init_secret_reward(*(++data), *(++data));
        else
          data += 2;
        break;
      case EntityScoreEntry:
        init_score_entry(fail_count);
        break;
      default:
        // We shouldn't ever hit this branch if our levels are crafted correctly
        // Just hard-lock
        while (1) {}
    }
    data++;
  }
}

void draw_level_name(const char *name) {
    init_text();
    text_cursor_x = 1;
    text_cursor_y = 7;
    text_color = TEXT_COLOR_WHITE;
    print_text((char *)name);
}

unsigned short inc_bcd(unsigned short n) {
  if ((n & 0x000F) != 0x0009) return n + 0x0001;
  if ((n & 0x00F0) != 0x0090) return (n + 0x0010) & 0xFFF0;
  if ((n & 0x0F00) != 0x0900) return (n + 0x0100) & 0xFF00;
  if ((n & 0xF000) != 0x9000) return n + 0x1000 & 0xF000;

  return 0x9999;
}

void draw_fail_count() {
  draw_box(1, 16, 126, 8, 0);
  await_draw_queue();
  init_text();
  text_cursor_x = 1;
  text_cursor_y = 16;
  text_color = TEXT_COLOR_WHITE;
  print_text("Fails ");
  print_bcd(fail_count);
}

void init_level() {
  LevelData l;
  l = levels[current_level];

  init_tilemap(l.tilemap, l.tilemap_decor);
  needs_draw_full_level = 2;

  init_entities(l.entities);
}

void reset_level() {
  LevelData *l;
  l = &levels[current_level];

  reset_player(l->reset_data[0], l->reset_data[1]);
  tilemap_reset_secret();
}

void init_game() {
  major_mode = ModeGame;
  current_level = STARTING_LEVEL;
  fail_count = 0;
  secrets_collected = 0;

  needs_draw_fail_count = 2;
  needs_draw_full_level = 2;

  init_level();
}

void (*const drawing_fns[])(char) = {
  (void (*const)(char))noop,
  draw_player,
  draw_hblockgroup,
  draw_loopboy,
  draw_boxpatrol,
  draw_secret,
  draw_menu,
  draw_secret_reward,
  draw_score_entry,
};

CollisionResult (*const test_collision[])(char) = {
  (CollisionResult (*const)(char))noop_collision,
  update_player,
  collision_hblockgroup,
  collision_loopboy,
  collision_boxpatrol,
  collision_secret,
  (CollisionResult (*const)(char))noop_collision,
  (CollisionResult (*const)(char))noop_collision,
  update_score_entry,
};

void (*const update_fns[])(char) = {
  (void (*const)(char))noop,
  (void (*const)(char))noop,
  update_hblockgroup,
  update_loopboy,
  update_boxpatrol,
  (void (*const)(char))noop,
  update_menu,
  update_secret_reward,
  (void (*const)(char))noop,
};

#define complete_level()                              \
  do {                                                \
    if (current_level == LEVEL_COUNT - 1) {           \
      init_game();                                    \
    } else {                                          \
      current_level++;                                \
      needs_draw_fail_count = 2;                      \
      init_level();                                   \
    }                                                 \
  } while(0);

int main() {
  char i;

  init_scores();

  init_graphics();
  load_font(0);

  init_dynawave();
  init_music();

  play_song(&ASSET__music__avhg_mid, REPEAT_LOOP);

  change_rom_bank(BANK_PROG0);

  flip_pages();
  await_draw_queue();
  flip_pages();
  await_draw_queue();

  // Level set up
  levels[0].tilemap = level_menu;
  levels[0].tilemap_decor = level_menu_decor;
  levels[0].entities = level_menu_entities;
  levels[0].reset_data = level_menu_reset_data;
  levels[0].name = level_menu_name;

  levels[1].tilemap = level_one;
  levels[1].tilemap_decor = level_one_decor;
  levels[1].entities = level_one_entities;
  levels[1].reset_data = level_one_reset_data;
  levels[1].name = level_one_name;

  levels[2].tilemap = level_two;
  levels[2].tilemap_decor = level_two_decor;
  levels[2].entities = level_two_entities;
  levels[2].reset_data = level_two_reset_data;
  levels[2].name = level_two_name;

  levels[3].tilemap = level_three;
  levels[3].tilemap_decor = level_three_decor;
  levels[3].entities = level_three_entities;
  levels[3].reset_data = level_three_reset_data;
  levels[3].name = level_three_name;

  levels[4].tilemap = level_four;
  levels[4].tilemap_decor = level_four_decor;
  levels[4].entities = level_four_entities;
  levels[4].reset_data = level_four_reset_data;
  levels[4].name = level_four_name;

  levels[5].tilemap = level_gg;
  levels[5].tilemap_decor = level_gg_decor;
  levels[5].entities = level_gg_entities;
  levels[5].reset_data = level_gg_reset_data;
  levels[5].name = level_gg_name;

  levels[6].tilemap = level_scores;
  levels[6].tilemap_decor = level_scores_decor;
  levels[6].entities = level_scores_entities;
  levels[6].reset_data = level_scores_reset_data;
  levels[6].name = level_scores_name;

  init_game();
  // Run forever
main_loop:
  while (1) {
    tick_music();

    update_inputs();

    switch (major_mode) {
      case ModeCredits:
        run_credits();
        break;
      case ModeScores:
        run_scores();
        break;
      case ModeGame:
        if (death_freeze == 0) {
          for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
            switch (test_collision[entities[i]](i)) {
              case ResultOk:
                break;
              case ResultFail:
                play_sound_effect(&ASSET__sfx__fail_bin, 2);
                death_freeze = MAX_DEATH_FREEZE;
                goto main_loop;
              case ResultWin:
                play_sound_effect(&ASSET__sfx__win_bin, 2);
                complete_level();
                goto main_loop;
              case ResultGetSecret:
                play_sound_effect(&ASSET__sfx__secret_bin, 2);
                tilemap_get_secret();
                break;
              case ResultSecretWin:
                play_sound_effect(&ASSET__sfx__win_bin, 2);
                secrets_collected++;
                complete_level();
                goto main_loop;
            }
          }

          for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
            update_fns[entities[i]](i);
          }
        } else {
          death_freeze--;

          if (death_freeze == 0) {
            fail_count = inc_bcd(fail_count);
            reset_level();
            needs_draw_fail_count = 2;
          }
        }

        if (needs_draw_full_level > 0) {
          draw_tilemap_full();
          draw_level_name(levels[current_level].name);

          needs_draw_full_level--;
        } else {
          draw_tilemap_partial();
        }


        if (needs_draw_fail_count > 0) {
              draw_fail_count();
              needs_draw_fail_count--;
        }

        for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
          // Draw entities in reverse order such that players are drawn on top of other entities
          drawing_fns[entities[i]](i);
        }
        break;
    }

    await_draw_queue();
    sleep(1);
    flip_pages();
  }

  //  We should never get here!
  return 0;
}
