#include "rbtree.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct node* rbt_create_node(char *word) {
    struct node *temp = malloc(sizeof(struct node));
    temp->word = word;
    temp->left = temp->right = temp->parent = NULL;
    temp->color = 1; // New node is red
    return temp;
}

static int compare(const char *a, const char *b) {
    return strcmp(a, b);
}

struct node* rbt_insert(struct node *root, struct node *temp) {
    if (!root) return temp;

    int cmp = compare(temp->word, root->word);
    if (cmp < 0) {
        root->left = rbt_insert(root->left, temp);
        root->left->parent = root;
    } else if (cmp > 0) {
        root->right = rbt_insert(root->right, temp);
        root->right->parent = root;
    } else {
        temp->word = NULL;
    }

    return root;
}

static void rotate_left(struct node **root, struct node *x) {
    struct node *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rotate_right(struct node **root, struct node *x) {
    struct node *y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) *root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void rbt_fix_insert(struct node *root, struct node *pt) {
    struct node *parent = NULL;
    struct node *grandparent = NULL;

    while ((pt != root) && (pt->color != 0) && (pt->parent->color == 1)) {
        parent = pt->parent;
        grandparent = pt->parent->parent;

        if (parent == grandparent->left) {
            struct node *uncle = grandparent->right;
            if (uncle && uncle->color == 1) {
                grandparent->color = 1;
                parent->color = uncle->color = 0;
                pt = grandparent;
            } else {
                if (pt == parent->right) {
                    rotate_left(&root, parent);
                    pt = parent;
                    parent = pt->parent;
                }
                rotate_right(&root, grandparent);
                int tmp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tmp;
                pt = parent;
            }
        } else {
            struct node *uncle = grandparent->left;
            if (uncle && uncle->color == 1) {
                grandparent->color = 1;
                parent->color = uncle->color = 0;
                pt = grandparent;
            } else {
                if (pt == parent->left) {
                    rotate_right(&root, parent);
                    pt = parent;
                    parent = pt->parent;
                }
                rotate_left(&root, grandparent);
                int tmp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tmp;
                pt = parent;
            }
        }
    }
    root->color = 0;
}

bool rbt_search(struct node *root, const char *word) {
    while (root) {
        int cmp = compare(word, root->word);
        if (cmp == 0) return true;
        root = (cmp < 0) ? root->left : root->right;
    }
    return false;
}

void rbt_inorder_print(struct node *root) {
    if (!root) return;
    rbt_inorder_print(root->left);
    printf("%s\n", root->word);
    rbt_inorder_print(root->right);
}

void rbt_filter_to_list(struct node *root, struct llNode **head, struct llNode **tail, bool (*f)(const char*), int *count) {
    if (!root) return;
    rbt_filter_to_list(root->left, head, tail, f, count);
    if (f(root->word)) {
        struct llNode *node = ll_create_node(root->word);
        ll_insert_last(head, tail, node);
        (*count)++;
    }
    rbt_filter_to_list(root->right, head, tail, f, count);
}

void rbt_free(struct node *root) {
    if (!root) return;
    rbt_free(root->left);
    rbt_free(root->right);
    free(root->word);
    free(root);
}
