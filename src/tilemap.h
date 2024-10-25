#ifndef TILEMAP_H_
#define TILEMAP_H_

#include "common.h"

#include "gt/drawing_funcs.h"
#include "gt/gametank.h"
#include "gt/banking.h"
#include "gt/drawing_funcs.h"

#include "camera.h"

void init_tilemap(unsigned short map_size, const unsigned char *map);
void draw_tilemap();

extern char *tilemap;

#endif // TILEMAP_H_
