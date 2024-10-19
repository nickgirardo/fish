#include "scores.h"

#include <string.h>
#include "common.h"

#include "gt/gametank.h"
#include "gt/drawing_funcs.h"
#include "gt/banking.h"
#include "gt/persist.h"
#include "gt/input.h"
#include "gt/music.h"
#include "gt/feature/text/text.h"

#include "gen/assets/sfx.h"

#define MAGIC_NUMBER 42

ScoreEntry *scores;
char *name;

unsigned char needs_draw;

#pragma bss-name (push, "SAVE")
char saved_magic_number;
ScoreEntry saved_normal_scores[SCORE_ENTRIES];
ScoreEntry saved_secret_scores[SCORE_ENTRIES];
#pragma bss-name (pop)

char get_new_score_index(ScoreEntry *score_table, unsigned short score) {
  signed char i;
  // Default ret to -1, it will be updated as we find new better scores
  // -1 will be returned if no scores are better than the new score
  signed char ret = -1;

  for (i = SCORE_ENTRIES - 1; i >= 0; i--) {
    if (score < score_table[i].score) {
      ret = i;
    } else {
      return ret;
    }
  }

  return ret;
}

// Copy the scores from normal RAM to persistant storage
void persist_scores() {
  char i = MAGIC_NUMBER;

  clear_save_sector();
  save_write(&i, &saved_magic_number, sizeof(unsigned char));

  save_write(&normal_scores, &saved_normal_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
  save_write(&secret_scores, &saved_secret_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
}

void save_new_score(ScoreEntry *score_table, unsigned short score, const char *name) {
  signed char score_ix, i;

  score_ix = get_new_score_index(score_table, score);

  // This new score is worse than all existing scores :(
  // It should not be saved so there is no work left to be done
  if (score_ix == -1)
    return;

  // Move all scores worse than the new score down a slot
  // This removes the old worst score from the list by overwriting it
  for (i = SCORE_ENTRIES - 2; i >= score_ix; i--) {
    memcpy(&score_table[i + 1], &score_table[i], sizeof(ScoreEntry));
  }

  // Write the new score into its slot
  score_table[score_ix].score = score;
  memcpy(score_table[score_ix].name, name, SCORE_NAME_LENGTH * sizeof(char));
}

void init_scores() {
  unsigned char i;

  change_rom_bank(SAVE_BANK_NUM);
  if (saved_magic_number == MAGIC_NUMBER) {
    // load saved scores
    memcpy(&normal_scores, &saved_normal_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
    memcpy(&secret_scores, &saved_secret_scores, SCORE_ENTRIES * sizeof(ScoreEntry));
  } else {
    normal_scores[0].score = 0x9999;
    normal_scores[0].name[0] = '.';
    normal_scores[0].name[1] = '.';
    normal_scores[0].name[2] = '.';
    normal_scores[0].name[3] = '.';
    normal_scores[0].name[4] = '.';
    normal_scores[0].name[5] = '.';
    normal_scores[0].name[6] = '.';
    normal_scores[0].name[7] = '.';
    normal_scores[0].name[8] = '.';
    normal_scores[0].name[9] = '.';

    secret_scores[0].score = 0x9999;
    secret_scores[0].name[0] = '.';
    secret_scores[0].name[1] = '.';
    secret_scores[0].name[2] = '.';
    secret_scores[0].name[3] = '.';
    secret_scores[0].name[4] = '.';
    secret_scores[0].name[5] = '.';
    secret_scores[0].name[6] = '.';
    secret_scores[0].name[7] = '.';
    secret_scores[0].name[8] = '.';
    secret_scores[0].name[9] = '.';

    for (i = 1; i < SCORE_ENTRIES; i++) {
      memcpy(&normal_scores[i], &normal_scores[0], sizeof(ScoreEntry));
      memcpy(&secret_scores[i], &secret_scores[0], sizeof(ScoreEntry));
    }

    persist_scores();
  }
  pop_rom_bank();
}

void enter_score_mode(ScoreEntry *_scores, char *_name) {
    major_mode = ModeScores;
    scores = _scores;
    name = _name;

    needs_draw = 2;
}

void run_scores() {
    unsigned char i, j;

    // Finished with checking out scores
    if (player1_new_buttons & INPUT_MASK_START || player1_new_buttons & INPUT_MASK_A) {
      play_sound_effect(&ASSET__sfx__menu_bin, 2);
      init_game();
    }

    if (needs_draw == 0)
      return;

    needs_draw--;

    clear_screen(0);
    clear_border(0);
    await_draw_queue();

    init_text();
    text_color = TEXT_COLOR_WHITE;

    text_cursor_y = 10;
    text_cursor_x = 10;
    print_text(name);

    text_cursor_y = 22;

    for (i = 0; i < SCORE_ENTRIES; i++) {
        for (j = 0; j < SCORE_NAME_LENGTH; j++) {
            text_cursor_x = 4 + (j<<3);
            print_char(scores[i].name[j]);
        }
        text_cursor_x = 92;
        print_bcd_zp(scores[i].score);

        text_cursor_y += 10;
    }
}

