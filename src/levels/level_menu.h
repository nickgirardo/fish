#ifndef LEVEL_MENU_H_
#define LEVEL_MENU_H_

#include "../common.h"

#pragma rodata-name (push, "PROG0")

const unsigned char level_menu[TILEMAP_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 5, 6, 0x14, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 5, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

const unsigned char level_menu_decor[] = {
24,
1, 24, 126, 15,
1, 39, 14, 88,
15, 49, 64, 78,
79, 113, 48, 14,
121, 39, 6, 10,
105, 49, 22, 64,
};

const unsigned char level_menu_entities[] = {
EntityMenu,
EntityHBlockGroup, 81, 41, 2, 64, 1, 0,
EntityHBlockGroup, 89, 105, 1, (unsigned char) -64, 1, 0,
EntitySecret, 92, 107,
EntityEmpty,
};

const unsigned char level_menu_reset_data[] = {
21,41,
};

const char level_menu_name[] = "A Very Hard Game";

#pragma rodata-name (pop)

#endif // LEVEL_MENU_H_
