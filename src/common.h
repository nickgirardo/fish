#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

#define ENTITY_TABLE_SIZE 10

// Mostly defining this for debugging purposes
#define STARTING_LEVEL 0

#define LEVEL_COUNT 7

#define COLOR_BG 6

// Tilemap related definitions
#define TILE_SIZE 8

// TODO tilemap width and height aren't going to be fixed as they were in avhg
#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 16
#define TILEMAP_SIZE (TILEMAP_HEIGHT * TILEMAP_WIDTH)

// TODO just setting a fairly large max for now
#define MAX_TILEMAP_SIZE 2048

#define TILE_WALL 0

#define SCORE_ENTRIES 10
#define SCORE_NAME_LENGTH 10

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

typedef enum {
  ResultOk = 0,
  ResultFail,
  ResultWin,
  ResultGetSecret,
  ResultSecretWin,
} CollisionResult;

typedef struct LevelDataT {
  const unsigned char *tilemap;
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
} EntityKind;

typedef struct PlayerDataT {
  Coord vx;
  Coord vy;
  unsigned char facing;
  unsigned char stroke_boost;
  unsigned char auto_sink;
  // Caching these values for faster collision detection
  unsigned char r;
  unsigned char d;
} PlayerData;

typedef struct ScoreEntryDataT {
  unsigned short score;
  unsigned char cursor;
  char entry[SCORE_NAME_LENGTH];
} ScoreEntryData;

typedef union EntityInnerDataU {
  PlayerData pd;
} EntityInnerData;

typedef struct EntityDataT {
  Coord x;
  Coord y;
  EntityInnerData data;
} EntityData;

extern EntityKind entities[ENTITY_TABLE_SIZE];
extern PlayerData *player_data;
extern EntityData entity_data[ENTITY_TABLE_SIZE];
extern unsigned char tilemap[MAX_TILEMAP_SIZE];
extern unsigned char tilemap_decor[64];
extern void init_game();

#endif // COMMON_H_
