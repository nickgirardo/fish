#include "score_display.h"

#include "common.h"
#include "levels/level_test.h"

void update_score_display() {
  level_test_x_plane[SCORE_DISPLAY_SCORE_START] = (player_data->data.pd.score & 0xF0) >> 1;
  level_test_x_plane[SCORE_DISPLAY_SCORE_START + 1] = (player_data->data.pd.score & 0x0F) << 3;

  level_test_x_plane[SCORE_DISPLAY_TIMER_START] = (seconds_remaining & 0xF0) >> 1;
  level_test_x_plane[SCORE_DISPLAY_TIMER_START + 1] = (seconds_remaining & 0x0F) << 3;
}
