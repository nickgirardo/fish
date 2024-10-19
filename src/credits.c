#include "credits.h"

#include "gt/drawing_funcs.h"
#include "gt/input.h"
#include "gt/music.h"
#include "gt/feature/text/text.h"

#include "common.h"

#include "../gen/assets/sfx.h"

#pragma code-name (push, "PROG0")

char credits_stage;

void init_credits() {
    major_mode = ModeCredits;
    credits_stage = 0;
}

void run_credits() {
  static char needs_draw = 2;

  if (needs_draw > 0) {
    needs_draw--;

    clear_screen(0);
    clear_border(0);
    await_draw_queue();

    init_text();
    text_print_line_start = 2;
    text_color = TEXT_COLOR_WHITE;

    set_cursor(2,9);

    switch (credits_stage) {
    case 0:
      print_text("A Very Hard Game\n\n\rCreated by Nick Girardo\n\n\rInspired by\n\r\"World's\n\rHardest Game\"\n\n\n\n\n\n\rv-rc4");
      break;
    case 1:
      print_text("Music:\n\n\rBlackfish \"Hapy Dangerous\"\n\n\rCC BY-SA");
      break;
    case 2:
      print_text("Special Thanks:\n\n\rClyde Shaffer\n\rTons of help\n\rwith everything\n\n\rdarthbeep\n\rPlay testing and\n\rfeedback\n\n\rHackManhattan");
      break;
    case 3:
      print_text("why do they call\n\rit oven when you\n\rof in the cold\n\rfood of out the\n\rhot eat the food");
      break;
    }
  }

  if (player1_new_buttons & (INPUT_MASK_A | INPUT_MASK_START)) {
    needs_draw = 2;
    play_sound_effect(&ASSET__sfx__menu_bin, 2);

    switch (credits_stage) {
    case 0:
      credits_stage = 1;
      break;
    case 1:
      credits_stage = 2;
      break;
    case 2:
      if (player1_buttons & INPUT_MASK_C) {
        credits_stage = 3;
      } else {
        init_game();
      }
      break;
    case 3:
      init_game();
      break;
    }
  }
}

#pragma code-name (pop)
