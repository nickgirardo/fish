#ifndef COMMON_H_
#define COMMON_H_

#include <stdbool.h>

#define ENTITY_TABLE_SIZE 10

// Mostly defining this for debugging purposes
#define STARTING_LEVEL 0

#define LEVEL_COUNT 7

#define MAX_SECRETS 5

#define COLOR_BG 6

// Tilemap related definitions
#define TILE_SIZE 8
#define TILEMAP_WIDTH 16
#define TILEMAP_HEIGHT 16
#define TILEMAP_SIZE (TILEMAP_HEIGHT * TILEMAP_WIDTH)

#define TILE_WALL 0
#define TILE_KILL 0xBB
#define TILE_GOAL 0x14
#define TILE_GOAL_SECRET 0xFC

#define SCORE_ENTRIES 10
#define SCORE_NAME_LENGTH 10

typedef enum {
  ModeGame,
  ModeCredits,
  ModeScores,
} MajorMode;

typedef enum { DirLeft, DirRight, DirUp, DirDown } Direction;

#define rand_direction() (rnd() & 3)

#define box_collision_x(L1, R1, L2, R2) (L1 < R2 && R1 > L2)
#define box_collision_y(U1, D1, U2, D2) (U1 < D2 && D1 > U2)

#define box_collision(L1, R1, U1, D1, L2, R2, U2, D2) \
  (box_collision_x(L1, R1, L2, R2) &&                 \
   box_collision_y(U1, D1, U2, D2))

#define point_collision(X, Y, L, R, U, D) \
  ((X > L) && (X < R) && (Y > U) && (Y < D))

typedef enum {
  MenuPlay,
  MenuScores,
  MenuCredits
} MenuOption;

typedef enum {
  ResultOk = 0,
  ResultFail,
  ResultWin,
  ResultGetSecret,
  ResultSecretWin,
} CollisionResult;

typedef struct LevelDataT {
  const unsigned char *tilemap;
  const unsigned char *tilemap_decor;
  const unsigned char *entities;
  const unsigned char *reset_data;
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
  EntityHBlockGroup,
  EntityLoopBoy,
  EntityBoxPatrol,
  EntitySecret,
  EntityMenu,
  EntitySecretReward,
  EntityScoreEntry,
} EntityKind;

typedef struct PlayerDataT {
  Coord x;
  Coord y;
  Coord vx;
  Coord vy;
  // Caching these values for faster collision detection
  unsigned char r;
  unsigned char d;
} PlayerData;

typedef struct HBlockGroupDataT {
  Coord x;
  Coord y;
  unsigned short vel;
  char n;
  char d_total;
  char d_remaining;
} HBlockGroupData;


typedef struct LoopBoyDataT {
  Coord x;
  Coord y;
  char n;
  char d;
  char direction;
} LoopBoyData;

typedef struct BoxPatrolDataT {
  Coord x;
  Coord y;
  char w;
  char h;
  char dir;
  char offset;
} BoxPatrolData;

typedef struct SecretDataT {
  char x;
  char y;
  bool collected;
} SecretData;

typedef struct SecretRewardDataT {
  char x;
  char y;
  bool visited;
} SecretRewardData;

typedef struct MenuDataT {
  MenuOption selection;
  char blink_timer;
  bool completed;
} MenuData;

typedef struct ScoreEntryDataT {
  unsigned short score;
  unsigned char cursor;
  char entry[SCORE_NAME_LENGTH];
} ScoreEntryData;

typedef union EntityDataU {
  PlayerData pd;
  HBlockGroupData hbgd;
  LoopBoyData lbd;
  SecretData sd;
  SecretRewardData srd;
  ScoreEntryData sed;
} EntityData;

typedef struct ScoreEntryT {
  unsigned short score;
  char name[SCORE_NAME_LENGTH];
} ScoreEntry;

extern MajorMode major_mode;
extern EntityKind entities[ENTITY_TABLE_SIZE];
extern PlayerData *player_data;
extern EntityData entity_data[ENTITY_TABLE_SIZE];
extern unsigned char tilemap[TILEMAP_SIZE];
extern unsigned char tilemap_decor[64];
extern void init_game();
extern void reset_level();

extern ScoreEntry normal_scores[SCORE_ENTRIES];
extern ScoreEntry secret_scores[SCORE_ENTRIES];

extern unsigned char secrets_collected;

#endif // COMMON_H_
