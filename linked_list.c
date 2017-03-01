/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Linked list data structure.
 * The code up to and including the dequeue function
 * are based heavily on pp. 171-173 of the book
 * 'Programming, Problem Solving, and Abstraction with C
 * Revised Edition, 2013' by Alistair Moffat.
 */

#include "linked_list.h"
#include <assert.h>
#include <stdlib.h>


list_t *make_empty_list(void) {
    list_t *list;
    list = malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;
    return list;
}

int is_empty(list_t *list) {
    assert(list!=NULL);
    return list->head == NULL;
}

void free_list(list_t *list) {
    node_t *curr, *prev;
    assert(list!=NULL);
    curr = list->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(list);
}

void enqueue(list_t *list, void *value) {
    node_t *new;
    new = malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->data = value;
    new->next = NULL;
    if (list->foot==NULL) {
        // First insertion into list
        list->head = list->foot = new;
    }
    else {
        list->foot->next = new;
        list->foot = new;
    }
}

void *peek(list_t *list) {
    assert(list!=NULL && list->head!=NULL);
    return list->head->data;
}

void *dequeue(list_t *list) {
    node_t *oldhead;
    void *oldhead_data = peek(list);
    assert(list!=NULL && list->head!=NULL);
    oldhead = list->head;
    list->head = list->head->next;
    if (list->head==NULL) {
        // There was only one node in the list
        list->foot = NULL;
    }
    free(oldhead);
    return oldhead_data;
}

int alter(list_t *list, void *value, int (*cmp)(void*,void*),
        void (*modify)(list_t*,node_t*,void*)) {
    node_t *curr, *prev;
    assert(list!=NULL);
    curr = list->head;
    if (!curr) {
        // List is empty
        modify(list, NULL, value);
        return 0;
    }
    if (cmp(curr->data, value)) {
        modify(list, NULL, value);
        return 1;
    }
    prev = curr;
    curr = curr->next;

    while (curr) {
        if (cmp(curr->data, value)) {
            modify(list, prev, value);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    modify(list, prev, value);
    return 0;
}

void insert(list_t *list, node_t *prev, void *value) {
    node_t *new;
    if (list->head==NULL) {
        // First insertion into list
        enqueue(list, value);
        return;
    }
    if (prev && !prev->next) {
        // Insert at foot
        enqueue(list, value);
        return;
    }

    new = malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->data = value;

    if (!prev) {
        // Insert to head
        new->next = list->head;
        list->head = new;
    } else if (prev->next) {
        new->next = prev->next;
        prev->next = new;
    }
}

void remove_node(list_t *list, node_t *prev, void *value) {
    node_t *curr;
    if (!prev && list->head) {
        // Removing head of list
        dequeue(list);
        return;
    }
    if (!prev) {
        // Empty list
        return;
    }
    curr = prev->next;
    if (!curr) {
        // cmp function failed
        return;
    }
    if (!curr->next) {
        // Removing foot of list
        list->foot = prev;
    }
    prev->next = curr->next;
    free(curr);
}

void *get_data(list_t *list, int (*condition)(void*)) {
    node_t *curr;
    assert(list!=NULL);
    curr = list->head;
    while (curr) {
        if (condition(curr->data)) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

int sum(list_t *list, int (*condition)(void*), int (*sum_func)(void*)) {
    int sum = 0;
    node_t *curr;
    assert(list!=NULL);
    curr = list->head;
    while (curr) {
        if (condition(curr->data)) {
            sum += sum_func(curr->data);
        }
        curr = curr->next;
    }
    return sum;
}

int count(list_t *list, int (*condition)(void*)) {
    return sum(list, condition, no_condition);
}

int no_condition(void *data) {
    return 1;
}
