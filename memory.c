/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Hole structure and memory management functions.
 */

#include "memory.h"
#include "process.h"
#include "linked_list.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * Allocates space and returns a hole with the
 * specified address and size.
 */
static hole_t *make_hole(int address, int size) {
    hole_t *hole;
    hole = malloc(sizeof(*hole));
    assert(hole!=NULL);
    assert(size > 0);
    hole->address = address;
    hole->size = size;
    return hole;
}

list_t *initialize_memory_holes(int size) {
    list_t *memory_holes;
    hole_t *hole;
    assert(size > 0);
    memory_holes = make_empty_list();

    // Start at highest address
    hole = make_hole(size, size);
    enqueue(memory_holes, hole);
    return memory_holes;
}

int in_memory(void *process) {
    process_t *p = process;
    if (p->time_inserted_in_memory >= 0) {
        return 1;
    }
    return 0;
}

int get_num_processes(list_t *waiting_processes) {
    // Add 1 for running process
    return count(waiting_processes, in_memory) + 1;
}

int get_num_holes(list_t *memory_holes) {
    return count(memory_holes, no_condition);
}

/*
 * Returns the memory size of the process.
 */
static int get_process_memory_size(void *process) {
    process_t *p = process;
    return p->memory_size;
}

/*
 * Returns the size of the hole.
 */
static int get_hole_size(void *hole) {
    hole_t *h = hole;
    return h->size;
}

int get_memory_usage(list_t *memory_holes, list_t *waiting_processes,
        process_t *running_process) {
    int usage = 0;
    int free = 0;
    double memory_usage;

    // Sum all memory sizes of processes in memory
    usage = sum(waiting_processes, in_memory, get_process_memory_size);
    if (running_process) {
        usage += running_process->memory_size;
    }
    // Sum all sizes of holes
    free = sum(memory_holes, no_condition, get_hole_size);
    memory_usage = (double)usage / (usage + free) * 100;
    return (int)ceil(memory_usage);
}

/*
 * Returns the largest process in waiting_processes that
 * is in memory. If there are multiple largest processes,
 * the process in memory longest is returned.
 * Assumes waiting_processes is not empty.
 */
static process_t *find_largest_process(list_t *waiting_processes) {
    node_t *curr;
    process_t *curr_process;
    process_t *largest_process;
    assert(waiting_processes!=NULL);
    assert(waiting_processes->head!=NULL);
    curr = waiting_processes->head;

    // Gets the first process in memory in the waiting list
    largest_process = (process_t *)get_data(waiting_processes, in_memory);
    while (curr) {
        curr_process = curr->data;
        // Process must be in memory
        if (curr_process->time_inserted_in_memory >= 0 &&
                curr_process->memory_size >= largest_process->memory_size) {

            // Remove process in memory longer if same size
            if (curr_process->memory_size != largest_process->memory_size ||
                    curr_process->time_inserted_in_memory <
                    largest_process->time_inserted_in_memory) {
                largest_process = curr_process;
            }
        }
        curr = curr->next;
    }
    return largest_process;
}

/*
 * Returns 1 if hole1 has a lower address than hole2,
 * 0 otherwise.
 */
static int has_lower_address(void *hole1, void *hole2) {
    hole_t *h1 = hole1;
    hole_t *h2 = hole2;
    if (h1->address < h2->address) {
        return 1;
    }
    return 0;
}

/*
 * Returns 1 if hole1 and hole2 have the same address,
 * 0 otherwise.
 */
static int has_same_address(void *hole1, void *hole2) {
    hole_t *h1 = hole1;
    hole_t *h2 = hole2;
    if (h1->address == h2->address) {
        return 1;
    }
    return 0;
}

/*
 * Returns 1 if hole1 and hole2 are adjacent, 0 otherwise.
 * Two holes are adjacent if the difference of their
 * addresses equals the hole size at the higher address.
 */
static int are_adjacent(void *hole1, void *hole2) {
    hole_t *h1 = hole1;
    hole_t *h2 = hole2;
    int address_diff = abs(h1->address - h2->address);
    if (h1->address > h2->address && address_diff == h1->size) {
        return 1;
    }
    if (h1->address < h2->address && address_diff == h2->size) {
        return 1;
    }
    return 0;
}

/*
 * For use with alter in linked_list.h as the modify function.
 * Merges adjacent holes, except when prev is NULL and the list
 * is empty or prev->next is NULL.
 * Assumes the input hole is a pointer to a hole_t that is not
 * inserted into memory.
 */
static void merge_adjacent_holes(list_t *list, node_t *prev, void *hole) {
    node_t *curr;
    hole_t *hole1;
    hole_t *hole2 = hole;
    if (!prev && list->head) {
        // Head is adjacent to value
        curr = list->head;
    } else if (!prev || !prev->next) {
        // No adjacent holes
        return;
    } else {
        curr = prev->next;
    }
    hole1 = curr->data;

    if (hole1->address > hole2->address) {
        hole2->address = hole1->address;
        // hole1 and hole2 must have different addresses
        // so that hole1 can be found by address and removed
        hole1->address = -1;
    }
    hole2->size += hole1->size;
    // Remove hole1 from the hole list
    alter(list, hole1, has_same_address, remove_node);
}

void remove_from_memory(list_t *memory_holes, process_t *process) {
    hole_t *hole;
    hole = make_hole(process->address, process->memory_size);

    if (is_empty(memory_holes)) {
        enqueue(memory_holes, hole);
    } else {
        // Insert hole according to address (highest to lowest)
        alter(memory_holes, hole, has_lower_address, insert);
    }

    // Try to merge the new hole with existing holes in the hole list
    while (alter(memory_holes, hole, are_adjacent, merge_adjacent_holes)) {
        // May have 3 holes to merge
    }
    process->address = -1;
    process->time_inserted_in_memory = -1;
}

/*
 * Returns the hole with the highest address in memory_holes
 * that is at least as large as process_size.
 * Returns NULL if no holes are large enough.
 */
static hole_t *find_first_fit(list_t *memory_holes, int process_size) {
    node_t *curr;
    hole_t *curr_hole;
    assert(memory_holes!=NULL);
    curr = memory_holes->head;

    while (curr) {
        curr_hole = curr->data;
        if (curr_hole->size >= process_size) {
            return curr_hole;
        }
        curr = curr->next;
    }
    return NULL;
}

void insert_in_memory(list_t *memory_holes, process_t *process,
        list_t *waiting_processes, int time) {
    process_t *largest_process;
    hole_t *suitable_hole;
    assert(process!=NULL);

    suitable_hole = find_first_fit(memory_holes, process->memory_size);
    while (!suitable_hole) {
        largest_process = find_largest_process(waiting_processes);
        remove_from_memory(memory_holes, largest_process);
        suitable_hole = find_first_fit(memory_holes, process->memory_size);
    }

    process->address = suitable_hole->address;
    process->time_inserted_in_memory = time;
    if (suitable_hole->size == process->memory_size) {
        // Remove the hole from the hole list
        alter(memory_holes, suitable_hole, has_same_address, remove_node);
    } else {
        suitable_hole->size -= process->memory_size;
        suitable_hole->address -= process->memory_size;
    }
}
