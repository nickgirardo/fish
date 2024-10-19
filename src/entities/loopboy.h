#ifndef LOOPBOYH_H_
#define LOOPBOYH_H_

#include "../common.h"

#define LOOPBOY_SIZE 6
#define LOOPBOY_COLOR 0xBB
#define LOOPBOY_DOUBLE_OFFSET 16
#define LOOPBOY_DOUBLE_OFFSET_LOG2 4
#define LOOPBOY_OFFSET 8

#define LOOPBOY_WRAPPING_FACTOR 0x7F

void init_loopboy(char x, char y, char n, Direction dir);
void draw_loopboy(char ix);
void update_loopboy(char ix);
CollisionResult collision_loopboy(char ix);


#endif // LOOPBOYH_H_
