#include "score_entry.h"

#include "../gt/input.h"
#include "../gt/music.h"
#include "../gt/feature/text/text.h"
#include "../gt/drawing_funcs.h"

#include "../gen/assets/sfx.h"

#include "../scores.h"

#pragma code-name (push, "PROG0")

#define ENTRY_CHAR_MAP_LENGTH 96

char entry_to_char[ENTRY_CHAR_MAP_LENGTH] = {
0x00, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65,
0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
0x76, 0x77, 0x78, 0x79, 0x7a, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x21,
0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x40, 0x3a,
0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x7b, 0x7c, 0x7d, 0x7e, 0x21, 0x60,
};

void init_score_entry(unsigned short score) {
  ScoreEntryData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityScoreEntry;

      data = (ScoreEntryData *) &entity_data[i];
      data->score = score;

      data->cursor = 0;

      data->entry[0] = 0;
      data->entry[1] = 0;
      data->entry[2] = 0;
      data->entry[3] = 0;
      data->entry[4] = 0;
      data->entry[5] = 0;
      data->entry[6] = 0;
      data->entry[7] = 0;
      data->entry[8] = 0;
      data->entry[9] = 0;

      return;
    }
  }

  // This should never be reached!!
}

void draw_score_entry(char ix) {
  ScoreEntryData data;
  unsigned char i;

  data = *((ScoreEntryData *) &entity_data[ix]);

  clear_screen(0);
  await_draw_queue();

  init_text();
  text_color = TEXT_COLOR_WHITE;

  text_cursor_x = 24;
  text_cursor_y = 40;
  print_text("GREAT JOB!");

  text_cursor_x = 8;
  text_cursor_y = 50;
  print_text("Enter your name");

  text_cursor_x = 24;
  text_cursor_y = 70;
  for (i = 0; i < SCORE_NAME_LENGTH; i++) {
    print_char(entry_to_char[data.entry[i]]);
  }

  text_cursor_x = 24 + (data.cursor << 3);
  text_cursor_y = 80;
  print_char('^');

  for (i = 0; i < SCORE_NAME_LENGTH; i++) {
    draw_box(24 + (i << 3), 78, 6, 1, 7);
  }

  await_draw_queue();

}

CollisionResult update_score_entry(char ix) {
  ScoreEntryData *data;
  char i;

  data = (ScoreEntryData *) &entity_data[ix];

  // Play a sfx if any of the relevant buttons are pressed
  if (player1_new_buttons &
      (INPUT_MASK_RIGHT |
       INPUT_MASK_LEFT |
       INPUT_MASK_UP |
       INPUT_MASK_DOWN |
       INPUT_MASK_A |
       INPUT_MASK_B |
       INPUT_MASK_START)) {
    play_sound_effect(&ASSET__sfx__menu_bin, 2);
  }

  if (player1_new_buttons & (INPUT_MASK_RIGHT | INPUT_MASK_A)) {
    // NOTE not doing bounds checking here! Below we will check if we've hit the end of the entry
    data->cursor++;
  } else if (player1_new_buttons & INPUT_MASK_B) {
    data->entry[data->cursor] = 0;
    if (data->cursor != 0)
      data->cursor--;
  } else if (player1_new_buttons & INPUT_MASK_LEFT) {
    if (data->cursor != 0)
      data->cursor--;
  } else if (player1_new_buttons & INPUT_MASK_UP) {
    if (data->entry[data->cursor] == ENTRY_CHAR_MAP_LENGTH - 1)
      data->entry[data->cursor] = 0;
    else
      data->entry[data->cursor]++;
  } else if (player1_new_buttons & INPUT_MASK_DOWN) {
    if (data->entry[data->cursor] == 0)
      data->entry[data->cursor] = ENTRY_CHAR_MAP_LENGTH - 1;
    else
      data->entry[data->cursor]--;
  }

  // Are we all done?
  if (player1_new_buttons & INPUT_MASK_START || data->cursor == SCORE_NAME_LENGTH) {
    // Convert the entry numbers to their actual char values
    for (i = 0; i < SCORE_NAME_LENGTH; i++) {
      data->entry[i] = entry_to_char[data->entry[i]];
    }

    // Always save scores to the normal scores table
    save_new_score(normal_scores, data->score, data->entry);

    // Only save scores to the secret scores table if all secrets have been collected
    if (secrets_collected == MAX_SECRETS)
	save_new_score(secret_scores, data->score, data->entry);

    persist_scores();

    // Return Win when we're done with name entry
    return ResultWin;
  }

  return ResultOk;
}

#pragma code-name (pop)
