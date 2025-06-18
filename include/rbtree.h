#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>

struct node {
    char *word;
    int color; // 1 = Red, 0 = Black
    struct node *left, *right, *parent;
};

struct node* rbt_create_node(char *word);
struct node* rbt_insert(struct node *root, struct node *new_node);
void rbt_fix_insert(struct node *root, struct node *pt);
bool rbt_search(struct node *root, const char *word);
void rbt_inorder_print(struct node *root);
void rbt_filter_to_list(struct node *root, struct llNode **head, struct llNode **tail, bool (*f)(const char*), int *count);
void rbt_free(struct node *root);

#endif
