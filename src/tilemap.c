#include "tilemap.h"
#include "common.h"
#include "gt/drawing_funcs.h"

#include "gt/gametank.h"
#include "gt/banking.h"
#include "gt/drawing_funcs.h"

#pragma code-name (push, "PROG0")

void init_tilemap(const unsigned char *map) {
    register unsigned char ix;

    ix = 0;

    do {
        tilemap[ix] = map[ix];
        ix++;
    } while(ix != (char) TILEMAP_SIZE);
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

void draw_tilemap() {
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
}

#pragma code-name (pop)
