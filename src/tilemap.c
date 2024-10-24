#include "tilemap.h"

#pragma code-name (push, "PROG0")

// TODO this is a big waste
void init_tilemap(unsigned short map_size, const unsigned char *map) {
    register unsigned short ix;

    for (ix = 0; ix < map_size; ++ix) {
        tilemap[ix] = map[ix];
    }
}

void draw_tilemap_real(unsigned char);

unsigned char *tm_tilemap;

void draw_tilemap() {
    PROFILER_START(5);
    *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE;
    draw_tilemap_real((unsigned char) (camera_x_scroll.hl.h));
    PROFILER_END(5);
}

#pragma code-name (pop)
