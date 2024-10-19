#ifndef LEVEL_THREE_H_
#define LEVEL_THREE_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_three[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0,
0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0,
0, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0,
0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 0,
0, 0x14, 0x14, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0,
0, 0x14, 0x14, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0,
0, 0x14, 0x14, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0,
0, 0x14, 0x14, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_three_decor[] = {
20,
1, 24, 126, 15,
1, 39, 6, 74,
1, 113, 126, 14,
121, 39, 6, 74,
7, 73, 88, 6,
};

const unsigned char level_three_entities[] = {
EntityHBlockGroup, 33, 41, 4, 40, 0, 0xA0,
EntityHBlockGroup, 41, 65, 4, (unsigned char) -40, 0, 0xA0,
EntityHBlockGroup, 97, 41, 2, 100, 0, 0xA0,
EntityHBlockGroup, 105, 105, 1, (unsigned char) -100, 0, 0xA0,
EntityHBlockGroup, 33, 81, 4, 40, 0, 0xA0,
EntityHBlockGroup, 41, 105, 4, (unsigned char) -40, 0, 0xA0,
EntitySecret, 97, 76,
EntityPlayer,12,44,
EntityEmpty,
};

const unsigned char level_three_reset_data[] = {
12,44,
};

const char level_three_name[] = "Level 3";

#pragma rodata-name (pop)

#endif // LEVEL_THREE_H_
