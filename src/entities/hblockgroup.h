#ifndef HBLOCKGROUP_H_
#define HBLOCKGROUP_H_

#include "../common.h"

#define HBLOCK_SIZE 6
#define HBLOCK_COLOR 0xBB
#define HBLOCK_OFFSET 16

// TODO just take a short instead of 2 chars
void init_hblockgroup(char x, char y, char n, char d, char vel_hi, char vel_lo);
void draw_hblockgroup(char ix);
void update_hblockgroup(char ix);
CollisionResult collision_hblockgroup(char ix);

#endif // HBLOCKGROUP_H_
