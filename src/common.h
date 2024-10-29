#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

#include "entities/ring_post.h"

#define ENTITY_TABLE_SIZE 10

#define BORDER_LEFT_WIDTH 8
#define BORDER_RIGHT_WIDTH 8
#define BORDER_TOP_HEIGHT 8
#define BORDER_BOTTOM_HEIGHT 1

#define LEVEL_COUNT 1
#define STARTING_LEVEL 0

// Tilemap related definitions
#define TILE_SIZE 8

typedef enum { DirLeft, DirRight, DirUp, DirDown } Direction;

#define rand_direction() (rnd() & 3)

#define box_collision_x(L1, R1, L2, R2) (L1 < R2 && R1 > L2)
#define box_collision_y(U1, D1, U2, D2) (U1 < D2 && D1 > U2)

#define box_collision(L1, R1, U1, D1, L2, R2, U2, D2) \
  (box_collision_x(L1, R1, L2, R2) &&                 \
   box_collision_y(U1, D1, U2, D2))

#define point_collision(X, Y, L, R, U, D) \
  ((X > L) && (X < R) && (Y > U) && (Y < D))

#define FACING_RIGHT 0
#define FACING_LEFT 1

// Empty structs aren't supported by cc65
// This struct with a single char is the smallest possible struct
typedef struct EmptyT {
  char unused;
} Empty;

typedef struct LevelDataT {
  const char *tilemap;
  const unsigned char *entities;
  const char *name;
} LevelData;

typedef struct HLT {
  char l;
  char h;
} HL;

typedef union CoordU {
  short c;
  HL hl;
} Coord;

typedef enum {
  EntityEmpty = 0,
  EntityPlayer,
  EntityRingPost,
} EntityKind;

typedef struct PlayerDataT {
  Coord vx;
  Coord vy;
  unsigned char facing;
  unsigned char stroke_boost;
  unsigned char auto_sink;
  bool is_left_of_ring;
  char score;
  // Caching these values for faster collision detection
  unsigned char r;
  unsigned char d;
  unsigned char mid_x;
  unsigned char ss_x;
} PlayerData;

typedef struct RingPostDataT {
  unsigned char mid_x;
  RingPostXSector x_sector;
  RingPostYSector y_sector;
} RingPostData;

typedef union EntityInnerDataU {
  PlayerData pd;
  RingPostData rpd;
  Empty empty;
} EntityInnerData;

typedef struct EntityDataT {
  Coord x;
  Coord y;
  EntityInnerData data;
} EntityData;

extern EntityKind entities[ENTITY_TABLE_SIZE];
extern EntityData entity_data[ENTITY_TABLE_SIZE];
extern EntityData *player_data;
extern EntityData *ring_post_data;
extern bool ring_collected;
extern void init_game();

#endif // COMMON_H_
