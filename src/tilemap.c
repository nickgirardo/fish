#include "tilemap.h"
#include "common.h"
#include "gt/drawing_funcs.h"

#include "gt/gametank.h"
#include "gt/banking.h"
#include "gt/drawing_funcs.h"

#pragma code-name (push, "PROG0")

void init_tilemap(const unsigned char *map, const unsigned char *decor) {
    register unsigned char ix;

    ix = 0;

    do {
        tilemap[ix] = map[ix];
        ix++;
    } while(ix != (char) TILEMAP_SIZE);

    for (ix = 0; ix < decor[0]+1; ix++) {
        tilemap_decor[ix] = decor[ix];
    }

}

#define PREP_DRAW_TILE()                      \
    do {                                      \
        *dma_flags = flagsMirror | DMA_COLORFILL_ENABLE | DMA_OPAQUE; \
        vram[VY] = 0;                         \
        vram[GX] = 0;                         \
        vram[GY] = 0;                         \
        vram[WIDTH] = 8;                      \
        vram[HEIGHT] = 8;                     \
    } while (0)

#define DRAW_TILE(x, c)                                           \
    do {                                                          \
    vram[VX] = x;                                                 \
    vram[COLOR] = ~c;                                             \
    vram[START] = 1;                                              \
} while (0)

void draw_tilemap_full() {
    register unsigned char i;
    register unsigned char x;
    register unsigned char y;

    x = 0;
    y = 0;
    i = 0;

    PREP_DRAW_TILE();

    do {
        DRAW_TILE(x, tilemap[i]);
        i++;

        x += TILE_SIZE;
        if (x == TILE_SIZE * TILEMAP_WIDTH) {
            x = 0;
            y += TILE_SIZE;
            vram[VY] = y;
        }

        // NOTE the blitter takes 1 cycle per pixel
        // Without these nops the blitter would not be finished before we queue
        // another blit operation
        // In the future we might want to use these cycles more usefully?
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    } while(i != (char) TILEMAP_SIZE);

    for (i = 1; i <= tilemap_decor[0];) {
        draw_box(tilemap_decor[i++], tilemap_decor[i++], tilemap_decor[i++], tilemap_decor[i++], COLOR_BG);
    }
    await_draw_queue();
}

void draw_tilemap_partial() {
    register unsigned char i;
    register unsigned char x;
    register unsigned char y;

    x = 0;
    y = 0;
    i = 0;

    PREP_DRAW_TILE();

    do {
        if (tilemap[i] != 0) {
            DRAW_TILE(x, tilemap[i]);
            // NOTE the blitter takes 1 cycle per pixel
            // Without these nops the blitter would not be finished before we queue
            // another blit operation
            // In the future we might want to use these cycles more usefully?
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
            asm("nop");
        }
        i++;

        x += TILE_SIZE;
        if (x == TILE_SIZE * TILEMAP_WIDTH) {
            x = 0;
            y += TILE_SIZE;
            vram[VY] = y;
        }

    } while(i != (char) TILEMAP_SIZE);
}

#define TILEMAP_REPLACE(FROM_TILE, TO_TILE)     \
do {                                            \
    register unsigned char ix;                  \
    ix = 0;                                     \
    do {                                        \
        if (tilemap[ix] == FROM_TILE)           \
            tilemap[ix] = TO_TILE;              \
        ix++;                                   \
    } while(ix != (char) TILEMAP_SIZE);         \
} while (0)


void tilemap_get_secret() {
    TILEMAP_REPLACE(TILE_GOAL, TILE_GOAL_SECRET);
}

void tilemap_reset_secret() {
    TILEMAP_REPLACE(TILE_GOAL_SECRET, TILE_GOAL);
}

#pragma code-name (pop)
