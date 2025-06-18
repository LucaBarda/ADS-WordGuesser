#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

void dictionary_init(int word_length);
char* dictionary_add_word(const char *word);
bool dictionary_word_exists(const char *word);
void dictionary_print_filtered(void);
void dictionary_add_compatible(const char *word);
void dictionary_sort_compatible(void);
void dictionary_reset_compatible(void);
void dictionary_update_compatible(bool (*is_compatible)(const char*));
int dictionary_get_total_words(void);
int dictionary_get_compatible_count(void);
void dictionary_free_all(void);

#endif
