#include "tilemap.h"

#pragma code-name (push, "PROG0")

char *tilemap;

// TODO this is a big waste
void init_tilemap(const char *map) {
    tilemap = map;
}

void draw_tilemap_real(unsigned char);

unsigned char *tm_tilemap;

void draw_tilemap() {
    *dma_flags = flagsMirror;
    draw_tilemap_real((unsigned char) (camera_scroll));
}

#pragma code-name (pop)
