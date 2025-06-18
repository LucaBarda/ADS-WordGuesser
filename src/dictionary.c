#include "dictionary.h"
#include "rbtree.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct node *rbt_root = NULL;
static struct llNode *comp_root = NULL;
static struct llNode *comp_last = NULL;

static int total_words = 0;
static int compatible_words = 0;
static int compatibility_phase = 1;
static int word_len = 0;

void dictionary_init(int word_length) {
    word_len = word_length;
}

char* dictionary_add_word(const char *word) {
    char *copy = malloc((word_len + 1) * sizeof(char));
    if (!copy) return NULL;
    strcpy(copy, word);

    struct node *node = rbt_create_node(copy);
    rbt_root = rbt_insert(rbt_root, node);
    if (node->word == NULL) {
        free(copy);
        free(node);
        return NULL;
    }

    rbt_fix_insert(rbt_root, node);
    total_words++;
    return node->word;
}

bool dictionary_word_exists(const char *word) {
    return rbt_search(rbt_root, word);
}

void dictionary_add_compatible(const char *word) {
    struct llNode *new_node = ll_create_node((char*)word);
    ll_insert_first(&comp_root, &comp_last, new_node);
    compatible_words++;
}

void dictionary_sort_compatible(void) {
    ll_merge_sort(&comp_root);
    comp_last = NULL; // not tracked after sort
}

void dictionary_print_filtered(void) {
    if (compatibility_phase == 1) {
        rbt_inorder_print(rbt_root);
    } else {
        ll_print(comp_root);
    }
}

void dictionary_reset_compatible(void) {
    ll_free_all(&comp_root);
    comp_last = NULL;
    compatible_words = 0;
    compatibility_phase = 1;
}

void dictionary_update_compatible(bool (*is_compatible)(const char*)) {
    if (compatibility_phase == 1) {
        rbt_filter_to_list(rbt_root, &comp_root, &comp_last, is_compatible, &compatible_words);
        compatibility_phase = 2;
    } else {
        ll_filter(&comp_root, &comp_last, is_compatible, &compatible_words);
    }
}

int dictionary_get_total_words(void) {
    return total_words;
}

int dictionary_get_compatible_count(void) {
    return compatible_words;
}

void dictionary_free_all(void) {
    rbt_free(rbt_root);
    rbt_root = NULL;
    total_words = 0;

    ll_free_all(&comp_root);
    comp_root = NULL;
    comp_last = NULL;
    compatible_words = 0;
    compatibility_phase = 1;
}
