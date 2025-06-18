#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dictionary.h"
#include "game.h"

#define MAX_INPUT 200

// Named constants for FSM states
#define STATE_WAITING_PARTITA      0
#define STATE_WAITING_REFERENCE    1
#define STATE_WAITING_ATTEMPTS     2
#define STATE_PLAYING              3
#define STATE_INSERTING_WORDS      4

int main() {
    int word_len;
    char input[MAX_INPUT];

    if (scanf("%d", &word_len) != 1 || word_len <= 0 || word_len >= MAX_INPUT) {
        return 0;
    }

    dictionary_init(word_len);
    game_init(word_len);

    // Initial loading of admissible words
    while (scanf("%s", input) == 1) {
        if (input[0] == '+') break;
        dictionary_add_word(input);
    }

    int state = STATE_WAITING_PARTITA;
    char *reference_word = NULL;
    int max_attempts = 0;
    int attempts = 0;

    int old_state = 0;
    int old_compatible = 0;

    do {
        if (strcmp(input, "+nuova_partita") == 0) {
            dictionary_reset_compatible();
            game_reset_comparison_state();
            game_reset_constraints();
            state = STATE_WAITING_REFERENCE;

        } else if (strcmp(input, "+stampa_filtrate") == 0) {
            dictionary_print_filtered();

        } else if (strcmp(input, "+inserisci_inizio") == 0) {
            old_state = state;
            old_compatible = dictionary_get_compatible_count();
            state = STATE_INSERTING_WORDS;

        } else if (strcmp(input, "+inserisci_fine") == 0 && state == STATE_INSERTING_WORDS) {
            if (dictionary_get_compatible_count() > old_compatible) {
                dictionary_sort_compatible();
            }
            state = old_state;

        } else if (state == STATE_WAITING_REFERENCE) {
            if (dictionary_word_exists(input)) {
                reference_word = strdup(input);
                game_learn_from_reference(reference_word);
                state = STATE_WAITING_ATTEMPTS;
            }

        } else if (state == STATE_WAITING_ATTEMPTS) {
            max_attempts = atoi(input);
            if (max_attempts > 0) {
                attempts = 0;
                state = STATE_PLAYING;
            }

        } else if (state == STATE_PLAYING) {
            if (!dictionary_word_exists(input)) {
                printf("not_exists\n");
            } else {
                int is_match = game_compare_and_print(input);
                dictionary_update_compatible(game_check_compatibility);
                printf("%d\n", dictionary_get_compatible_count());
                attempts++;

                if (is_match) {
                    free(reference_word);
                    reference_word = NULL;
                    state = STATE_WAITING_PARTITA;
                } else if (attempts == max_attempts) {
                    printf("ko\n");
                    free(reference_word);
                    reference_word = NULL;
                    state = STATE_WAITING_PARTITA;
                }
            }

        } else if (state == STATE_INSERTING_WORDS) {
            if (strlen(input) == word_len) {
                char *inserted = dictionary_add_word(input);
                if (old_state == STATE_PLAYING && inserted && game_check_compatibility(inserted)) {
                    dictionary_add_compatible(inserted);
                }
            }
        }
    } while (scanf("%s", input) == 1);

    dictionary_free_all();
    return 0;
}
