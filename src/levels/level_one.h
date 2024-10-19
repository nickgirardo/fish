#ifndef LEVEL_ONE_H_
#define LEVEL_ONE_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_one[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0x14, 0x14, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0x14, 0x14, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0, 0,
0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_one_decor[] = {
24,
1, 24, 126, 15,
1, 39, 30, 56,
1, 95, 6, 18,
1, 113, 126, 14,
121, 39, 6, 18,
97, 57, 30, 56,
};

const unsigned char level_one_entities[] = {
EntityHBlockGroup, 41, 41, 4, 64, 1, 0,
EntityHBlockGroup, 33, 105, 4, (unsigned char) -64, 1, 0,
EntitySecret, 69, 69,
EntityPlayer,12,100,
EntityEmpty,
};

const unsigned char level_one_reset_data[] = {
12,100,
};

const char level_one_name[] = "Level 1";

#pragma rodata-name (pop)

#endif // LEVEL_ONE_H_
