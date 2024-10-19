#ifndef SECRET_H_
#define SECRET_H_

#include "../common.h"

// For debugging purposes
// #define RENDER_SECRETS

void init_secret(char x, char y);

#ifdef RENDER_SECRETS
void draw_secret(char ix);
#endif

#ifndef RENDER_SECRETS
#define draw_secret ((void (*const)(char))noop)
#endif

CollisionResult collision_secret(char ix);


#endif // SECRET_H_
