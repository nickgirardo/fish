#include "tilemap.h"

#pragma code-name (push, "PROG0")

char *tilemap;

// TODO this is a big waste
void init_tilemap(unsigned short map_size, const unsigned char *map) {
    tilemap = map;
}

void draw_tilemap_real(unsigned char);

unsigned char *tm_tilemap;

void draw_tilemap() {
    PROFILER_START(5);
    *dma_flags = flagsMirror;
    draw_tilemap_real((unsigned char) (camera_scroll));
    PROFILER_END(5);
}

#pragma code-name (pop)
