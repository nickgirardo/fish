#include "menu.h"

#include "../gt/input.h"
#include "../gt/drawing_funcs.h"
#include "../gt/music.h"
#include "../gt/feature/text/text.h"

#include "../gen/assets/sfx.h"

#include "../credits.h"
#include "../scores.h"

#include "player.h"

#pragma code-name (push, "PROG0")

void init_menu() {
  MenuData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntityMenu;

      data = (MenuData *) &entity_data[i];

      data->selection = MenuPlay;
      data->blink_timer = 0;
      data->completed = false;

      return;
    }
  }
}

char menu_option_cursor_y(MenuOption m) {
    switch (m) {
        case MenuPlay:
            return 88;
        case MenuScores:
            return 96;
        case MenuCredits:
            return 104;
    }
}

void draw_menu(char ix) {
  MenuData data;

  data = *((MenuData *) &entity_data[ix]);

  // Clear last frame
  draw_box(8, 88, 64, 24, COLOR_BG);
  await_draw_queue();

  if (data.completed)
      return;

  init_text();
  text_cursor_x = 16;
  text_cursor_y = 88;
  print_text("Play");

  text_cursor_x = 16;
  text_cursor_y = 96;
  print_text("Scores");

  text_cursor_x = 16;
  text_cursor_y = 104;
  print_text("Credits");

  // Blink cursor
  if ((data.blink_timer + 32) & 96) {
    text_cursor_x = 8;
    text_cursor_y = menu_option_cursor_y(data.selection);
    print_text(">");
  }

  // draw_box(8, 56, 16, 16, 71);
}

MenuOption prev_menu_option(MenuOption m) {
    switch (m) {
        case MenuPlay:
            return MenuCredits;
        case MenuScores:
            return MenuPlay;
        case MenuCredits:
            return MenuScores;
    }
}

MenuOption next_menu_option(MenuOption m) {
    switch (m) {
        case MenuPlay:
            return MenuScores;
        case MenuScores:
            return MenuCredits;
        case MenuCredits:
            return MenuPlay;
    }
}

void update_menu(char ix) {
  MenuData *data;

  data = ((MenuData *) &entity_data[ix]);

  if (data->completed) {
      return;
  }

  data->blink_timer++;

  if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
      switch (data->selection) {
          case MenuPlay:
              play_sound_effect(&ASSET__sfx__menu_bin, 2);
              data->completed = true;
              // TODO hardcoding for now
              init_player(21, 41);
              break;
          case MenuScores:
              play_sound_effect(&ASSET__sfx__menu_bin, 2);
              if (player1_buttons & INPUT_MASK_C)
                enter_score_mode(secret_scores, "SECRET SCORES!");
              else
                enter_score_mode(normal_scores, "    SCORES!");
              break;
          case MenuCredits:
              play_sound_effect(&ASSET__sfx__menu_bin, 2);
              init_credits();
              break;
      }
  } else if (player1_new_buttons & INPUT_MASK_DOWN) {
    play_sound_effect(&ASSET__sfx__menu_bin, 2);
    data->blink_timer = 0;
    data->selection = next_menu_option(data->selection);
  } else if (player1_new_buttons & INPUT_MASK_UP) {
    play_sound_effect(&ASSET__sfx__menu_bin, 2);
    data->blink_timer = 0;
    data->selection = prev_menu_option(data->selection);
  }

}

#pragma code-name (pop)
