#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

void game_init(int word_length);
void game_reset_comparison_state(void);
void game_reset_constraints(void);
void game_learn_from_reference(const char *reference);
int game_compare_and_print(const char *word);
bool game_check_compatibility(const char *word);

#endif
