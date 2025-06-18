#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct llNode {
    char *word;
    struct llNode *next;
};

// Core operations
struct llNode* ll_create_node(char *word);
void ll_insert_first(struct llNode **head, struct llNode **tail, struct llNode *newNode);
void ll_insert_last(struct llNode **head, struct llNode **tail, struct llNode *newNode);
void ll_merge_sort(struct llNode **head);
void ll_print(struct llNode *head);
void ll_filter(struct llNode **head, struct llNode **tail, bool (*f)(const char*), int *count);
void ll_free_all(struct llNode **head);

#endif
