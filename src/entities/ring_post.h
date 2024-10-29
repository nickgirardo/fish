#ifndef RING_POST_H_
#define RING_POST_H_

#define RING_POST_SIZE 6
#define RING_POST_HALF_SIZE 3

#define RING_POST_GAP 28

// How many pixels offset from the top is the center point of the ring?
// Note this would actually be 15.5 as it's (RING_POST_HALF_SIZE + RING_POST_GAP)/2
#define RING_CENTER_OFFSET 15

typedef enum RingPostXSectorE {
  RingPostXLeft = 0,
  RingPostXMiddle,
  RingPostXRight,
} RingPostXSector;

typedef enum RingPostYSectorE {
  RingPostYTop = 0,
  RingPostYBottom,
} RingPostYSector;

void init_ring_post(char x, char y);

void update_ring_post(char ix);

// TODO for debugging, rm
void draw_ring_post(char ix);

#endif
