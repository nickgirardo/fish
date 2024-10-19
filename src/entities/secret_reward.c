#include "secret_reward.h"

#include "../gt/feature/text/text.h"
#include "player.h"

#pragma code-name (push, "PROG0")

void init_secret_reward(char x, char y) {
  SecretRewardData *data;
  char i;

  for (i = 0; i < ENTITY_TABLE_SIZE; i++) {
    if (entities[i] == EntityEmpty) {
      entities[i] = EntitySecretReward;

      data = (SecretRewardData *) &entity_data[i];

      data->x = x;
      data->y = y;
      data->visited = false;

      return;
    }
  }
}

void draw_secret_reward(char ix) {
  SecretRewardData data;

  data = *((SecretRewardData *) &entity_data[ix]);

  init_text();
  text_cursor_x = data.x;
  text_cursor_y = data.y;
  print_char('?');

  if (!data.visited)
      return;

  text_cursor_x = 2;
  text_cursor_y = 109;
  print_text("YOU ARE");

  text_cursor_x = 2;
  text_cursor_y = 119;
  print_text("A SUPER PLAYER!!");
}

void update_secret_reward(char ix) {
  SecretRewardData *data;

  data = ((SecretRewardData *) &entity_data[ix]);

  if (data->visited)
      return;

  if (box_collision(data->x,
                    data->x + SR_SIZE,
                    data->y,
                    data->y + SR_SIZE,
                    player_data->x.hl.h,
                    player_data->r,
                    player_data->y.hl.h,
                    player_data->d)) {
      data->visited = true;
  }
}

#pragma code-name (pop)
