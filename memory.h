/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Hole structure and memory management functions.
 */

#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "linked_list.h"
#include "process.h"

typedef struct {
    int address; // Highest number occupying in memory
    int size; // size > 0
} hole_t;

/*
 * Allocates space and returns a list with
 * a hole_t node of the specified size.
 */
list_t *initialize_memory_holes(int size);

/*
 * Returns 1 if the process is
 * in memory, 0 otherwise.
 */
int in_memory(void *process);

/*
 * Returns the number of processes in memory.
 * Assumes a process is currently running
 * (in memory and not in waiting_processes).
 */
int get_num_processes(list_t *waiting_processes);

/*
 * Returns the number of holes in memory.
 */
int get_num_holes(list_t *memory_holes);

/*
 * Returns the rounded up memory usage percentage.
 */
int get_memory_usage(list_t *memory_holes, list_t *waiting_processes,
        process_t *running_process);

/*
 * Removes process from memory, replacing
 * it with a hole in memory_holes.
 */
void remove_from_memory(list_t *memory_holes, process_t *process);

/*
 * Inserts process into memory. Finds the first fit in memory
 * to insert the process. Removes the largest hole repeatedly
 * until a hole is large enough for the process.
 * Assumes all processes require memory <= total memory.
 */
void insert_in_memory(list_t *memory_holes, process_t *process,
        list_t *waiting_processes, int time);

#endif /* MEMORY_H_INCLUDED */
