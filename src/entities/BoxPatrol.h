#ifndef BOXPATROL_H_
#define BOXPATROL_H_

#include "../common.h"

#define BOXPATROL_SIZE 6
#define BOXPATROL_COLOR 0xBB
#define BOXPATROL_DOUBLE_OFFSET 16
#define BOXPATROL_DOUBLE_OFFSET_LOG2 4
#define BOXPATROL_OFFSET 8

void init_boxpatrol(char x, char y, char w, char h, Direction dir, char offset);
void draw_boxpatrol(char ix);
void update_boxpatrol(char ix);
CollisionResult collision_boxpatrol(char ix);



#endif // BOXPATROL_H_
