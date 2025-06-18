#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char *positions;
    unsigned short total;
    unsigned short matched;
} SymbolInfo;

typedef struct {
    char *positions;
    unsigned short total;
    unsigned short total_correct;
    unsigned short matched;
    unsigned short count;
    char symbol;
    bool exact;
} Constraint;

static int k = 0;
static SymbolInfo *symbol_data[128];
static Constraint *constraints[128];
static unsigned short constraint_keys[128];
static unsigned short num_constraints = 0;

void game_init(int word_length) {
    k = word_length;
}

void game_reset_comparison_state(void) {
    for (int i = 0; i < 128; ++i) {
        if (symbol_data[i]) {
            free(symbol_data[i]->positions);
            free(symbol_data[i]);
            symbol_data[i] = NULL;
        }
    }
}

void game_reset_constraints(void) {
    for (int i = 0; i < 128; ++i) {
        if (constraints[i]) {
            free(constraints[i]->positions);
            free(constraints[i]);
            constraints[i] = NULL;
        }
        constraint_keys[i] = 0;
    }
    num_constraints = 0;
}

static Constraint* get_constraint(char ch) {
    return constraints[(unsigned char)ch];
}

static void add_constraint(Constraint *c) {
    unsigned short idx = (unsigned char)c->symbol;
    constraints[idx] = c;
    constraint_keys[num_constraints++] = idx;
}

void game_learn_from_reference(const char *ref) {
    game_reset_comparison_state();

    for (int i = 0; i < k; ++i) {
        char ch = ref[i];
        unsigned char idx = (unsigned char)ch;

        if (!symbol_data[idx]) {
            symbol_data[idx] = malloc(sizeof(SymbolInfo));
            symbol_data[idx]->positions = calloc(k, sizeof(char));
            symbol_data[idx]->total = 0;
            symbol_data[idx]->matched = 0;
        }
        symbol_data[idx]->positions[i] = 'X';
        symbol_data[idx]->total++;
    }
}

int game_compare_and_print(const char *guess) {
    game_reset_comparison_state(); // reset matched counts
    char *result = calloc(k + 1, sizeof(char));
    int *uncertain = malloc(k * sizeof(int));
    int exact = 0, u_count = 0;

    for (int i = 0; i < k; ++i) {
        char ch = guess[i];
        unsigned char idx = (unsigned char)ch;

        Constraint *c = get_constraint(ch);
        if (!c) {
            c = calloc(1, sizeof(Constraint));
            c->symbol = ch;
            add_constraint(c);
        }

        if (!symbol_data[idx]) {
            result[i] = '/';
        } else if (symbol_data[idx]->positions[i] == 'X') {
            result[i] = '+';
            symbol_data[idx]->matched++;
            exact++;

            if (!c->positions) c->positions = calloc(k, sizeof(char));
            if (c->positions[i] != 'X') {
                c->positions[i] = 'X';
                c->total_correct++;
            }
        } else {
            result[i] = '?';
            uncertain[u_count++] = i;
            if (!c->positions) c->positions = calloc(k, sizeof(char));
            c->positions[i] = '|';
        }
    }

    if (exact == k) {
        printf("ok\n");
        free(result);
        free(uncertain);
        return 1;
    }

    for (int j = 0; j < u_count; ++j) {
        int i = uncertain[j];
        char ch = guess[i];
        unsigned char idx = (unsigned char)ch;

        Constraint *c = get_constraint(ch);
        if (symbol_data[idx]->matched < symbol_data[idx]->total) {
            result[i] = '|';
            symbol_data[idx]->matched++;
        } else {
            result[i] = '/';
            c->exact = true;
        }

        if (symbol_data[idx]->matched > c->total) {
            c->total = symbol_data[idx]->matched;
        }
    }

    printf("%s\n", result);
    free(result);
    free(uncertain);
    return 0;
}

bool game_check_compatibility(const char *word) {
    for (int i = 0; i < num_constraints; ++i) {
        Constraint *c = constraints[constraint_keys[i]];
        c->matched = 0;
        c->count = 0;
    }

    for (int i = 0; i < k; ++i) {
        char ch = word[i];
        Constraint *c = get_constraint(ch);
        if (!c) continue;

        if (!c->positions) return false;

        if (c->positions[i] == '|') return false;
        if (c->positions[i] == 'X') c->matched++;
        if (c->total > 0) c->count++;
    }

    for (int i = 0; i < num_constraints; ++i) {
        Constraint *c = constraints[constraint_keys[i]];
        if (c->total_correct && c->matched != c->total_correct) return false;
        if (c->total && !c->exact && c->count < c->total) return false;
        if (c->exact && c->count != c->total) return false;
    }

    return true;
}
