#ifndef LEVEL_TWO_H_
#define LEVEL_TWO_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_two[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 5, 6, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,
0, 0x14, 0x14, 5, 6, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0x14, 0x14, 6, 5, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0x14, 0x14, 5, 6, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0,
0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 6, 5, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_two_decor[] = {
44,
1, 24, 126, 7,
1, 31, 30, 32,
1, 63, 6, 26,
1, 89, 30, 32,
1, 121, 126, 6,
105, 31, 22, 90,
41, 31, 46, 8,
49, 113, 46, 8,
79, 49, 16, 14,
79, 89, 16, 14,
41, 49, 38, 54,
};

const unsigned char level_two_entities[] = {
EntityLoopBoy, 33, 41, 6, DirLeft,
EntityLoopBoy, 97, 105, 6, DirRight,
EntityLoopBoy, 33, 105, 6, DirDown,
EntityLoopBoy, 97, 41, 6, DirUp,
EntitySecret, 100, 115,
EntityPlayer,84,73,
EntityEmpty,
};

const unsigned char level_two_reset_data[] = {
84,73,
};

const char level_two_name[] = "Level 2";

#endif // LEVEL_TWO_H_
