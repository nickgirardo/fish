#ifndef TILEMAP_H_
#define TILEMAP_H_

void init_tilemap(const unsigned char *map, const unsigned char *decor);
void draw_tilemap_full();
void draw_tilemap_partial();

void tilemap_get_secret();
void tilemap_reset_secret();

#endif // TILEMAP_H_
