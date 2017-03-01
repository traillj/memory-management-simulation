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

#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

typedef struct node node_t;

struct node {
    void *data;
    node_t *next; // Pointer to next node
};

typedef struct {
    node_t *head; // Pointer to head node
    node_t *foot; // Pointer to foot node
} list_t;

/*
 * Allocates space and returns an empty linked list.
 */
list_t *make_empty_list(void);

/*
 * Returns zero if there are no nodes in the list,
 * a non-zero number otherwise.
 */
int is_empty(list_t *list);

/*
 * Frees the space allocated to the list and its nodes.
 */
void free_list(list_t *list);

/*
 * Inserts a node with its data set to
 * value to the foot of the list.
 */
void enqueue(list_t *list, void *value);

/*
 * Returns the data of the node at the head of the
 * list without removing the node from the list.
 */
void *peek(list_t *list);

/*
 * Returns the data of the node at the head of the
 * list and removes the node from the list.
 */
void *dequeue(list_t *list);

/*
 * Calls the modify function on the node which has data
 * that is true in the cmp function with respect to value.
 * Always calls modify once.
 * If list is empty or the head node succeeds in cmp,
 * prev is set to NULL is modify.
 * If the cmp function never succeeds, the last node is
 * passed as prev in modify.
 * Returns 1 if cmp succeeds, 0 otherwise.
 */
int alter(list_t *list, void *value, int (*cmp)(void*,void*),
        void (*modify)(list_t*,node_t*,void*));

/*
 * For use with alter as the modify function.
 * Inserts a node with data set to value after the
 * node prev in list. Inserts at the head if prev is NULL.
 */
void insert(list_t *list, node_t *prev, void *value);

/*
 * For use with alter as the modify function.
 * Removes the node after prev.
 * Removes the head node if the list is not empty
 * and prev is NULL.
 */
void remove_node(list_t *list, node_t *prev, void *value);

/*
 * Returns the data that is the first to satisfy
 * the condition function.
 * Returns NULL if condition never succeeds.
 */
void *get_data(list_t *list, int (*condition)(void*));

/*
 * Calls the sum_func function with the node's data
 * each time the data satisfies the condition function.
 * Returns the sum of the return values of sum_func.
 */
int sum(list_t *list, int (*condition)(void*), int (*sum_func)(void*));

/*
 * Returns a count of the number of nodes with
 * data that satisfies the condition function.
 */
int count(list_t *list, int (*condition)(void*));

/*
 * For use with count or sum as the condition function.
 * Returns 1.
 */
int no_condition(void *data);

#endif /* LINKED_LIST_H_INCLUDED */
