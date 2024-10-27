#ifndef RING_POST_H_
#define RING_POST_H_

#include "../gt/gametank.h"

#include "../common.h"

#define RING_POST_SIZE 6
#define RING_POST_HALF_SIZE 3

void init_ring_post(char x, char y);
void move_ring_post(char ix, char x, char y);

// TODO for debugging, rm
void draw_ring_post(char ix);

#endif
