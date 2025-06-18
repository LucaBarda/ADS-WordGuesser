#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct llNode* ll_create_node(char *word) {
    struct llNode *node = malloc(sizeof(struct llNode));
    node->word = word;
    node->next = NULL;
    return node;
}

void ll_insert_first(struct llNode **head, struct llNode **tail, struct llNode *newNode) {
    newNode->next = *head;
    *head = newNode;
    if (*tail == NULL) *tail = newNode;
}

void ll_insert_last(struct llNode **head, struct llNode **tail, struct llNode *newNode) {
    newNode->next = NULL;
    if (*head == NULL) {
        *head = *tail = newNode;
    } else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

void ll_print(struct llNode *head) {
    while (head != NULL) {
        printf("%s\n", head->word);
        head = head->next;
    }
}

void ll_free_all(struct llNode **head) {
    struct llNode *current = *head;
    while (current != NULL) {
        struct llNode *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

void ll_split(struct llNode *source, struct llNode **front, struct llNode **back) {
    struct llNode *slow = source;
    struct llNode *fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

struct llNode* ll_sorted_merge(struct llNode *a, struct llNode *b) {
    if (!a) return b;
    if (!b) return a;

    struct llNode *result = NULL;

    if (strcmp(a->word, b->word) <= 0) {
        result = a;
        result->next = ll_sorted_merge(a->next, b);
    } else {
        result = b;
        result->next = ll_sorted_merge(a, b->next);
    }
    return result;
}

void ll_merge_sort(struct llNode **headRef) {
    struct llNode *head = *headRef;
    if (!head || !head->next) return;

    struct llNode *a, *b;
    ll_split(head, &a, &b);

    ll_merge_sort(&a);
    ll_merge_sort(&b);

    *headRef = ll_sorted_merge(a, b);
}

void ll_filter(struct llNode **head, struct llNode **tail, bool (*f)(const char*), int *count) {
    struct llNode *prev = NULL, *curr = *head;

    while (curr != NULL) {
        if (!f(curr->word)) {
            struct llNode *temp = curr;
            if (curr == *head) {
                *head = curr->next;
                curr = *head;
            } else {
                prev->next = curr->next;
                curr = prev->next;
            }
            if (temp == *tail) {
                *tail = prev;
            }
            free(temp);
            (*count)--;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }

    if (*head == NULL) *tail = NULL;
}
