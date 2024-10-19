#ifndef LEVEL_FOUR_H_
#define LEVEL_FOUR_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_four[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0,
0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0,
0, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 0, 0, 0,
0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0,
0, 5, 6, 0, 6, 5, 6, 0, 6, 0, 6, 0, 6, 0, 0, 0,
0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 0, 0,
0, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 0, 6, 0, 0, 0,
0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0x14, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 0x14, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_four_decor[] = {
60,
1, 24, 126, 15,
1, 39, 94, 8,
1, 47, 14, 24,
1, 71, 6, 10,
1, 81, 14, 46,
15, 105, 80, 8,
15, 113, 112, 14,
121, 39, 6, 74,
25, 57, 6, 38,
41, 57, 6, 14,
41, 81, 6, 14,
57, 57, 6, 38,
73, 57, 6, 38,
89, 57, 6, 38,
105, 57, 16, 38,
};

const unsigned char level_four_entities[] = {
EntityHBlockGroup, 33, 49, 5, 74, 0, 0xA0,
EntityBoxPatrol, 17, 49, 80, 48, DirDown, 0,
EntityBoxPatrol, 97, 97, 80, 48, DirUp, 0,
EntitySecret, 100, 86,
EntityPlayer,110,44,
EntityEmpty,
};

const unsigned char level_four_reset_data[] = {
110,44,
};

const char level_four_name[] = "Level 4";

#pragma rodata-name (pop)

#endif // LEVEL_FOUR_H_
