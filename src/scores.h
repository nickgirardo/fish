#ifndef SCORES_H_
#define SCORES_H_

#include "common.h"

void init_scores();

void enter_score_mode(ScoreEntry *scores, char *name);
void run_scores();

void save_new_score(ScoreEntry *score_table, unsigned short score, const char *name);
void persist_scores();

#endif // SCORES_H_
