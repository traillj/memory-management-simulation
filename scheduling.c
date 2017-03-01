/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Schedule structure and process scheduling methods.
 */

#include "scheduling.h"
#include "process.h"
#include <assert.h>
#include <stdlib.h>

/*
 * The time a process is given to run
 * on CPU for queues Q1, Q2 and Q3.
 */
#define QUANTUM_1 2
#define QUANTUM_2 4
#define QUANTUM_3 8


schedule_t *initialize_schedule(char scheduling_algorithm) {
    schedule_t *schedule;
    schedule = malloc(sizeof(*schedule));
    assert(schedule!=NULL);
    schedule->all = make_empty_list();

    if (scheduling_algorithm == 'm') {
        schedule->Q1 = make_empty_list();
        schedule->Q2 = make_empty_list();
        schedule->Q3 = make_empty_list();
    } else {
        schedule->Q1 = NULL;
        schedule->Q2 = NULL;
        schedule->Q3 = NULL;
    }
    schedule->quantum_timer = 0;
    return schedule;
}

void schedule_process(schedule_t *waiting_processes, process_t *process) {
    assert(waiting_processes!=NULL);
    enqueue(waiting_processes->all, process);
    if (waiting_processes->Q1) {
        enqueue(waiting_processes->Q1, process);
    }
}


process_t *schedule_fcfs(schedule_t *waiting_processes,
        process_t *running_process) {
    assert(waiting_processes!=NULL);
    if (!running_process && !is_empty(waiting_processes->all)) {
        return dequeue(waiting_processes->all);
    }
    return NULL;
}

/*
 * Removes the head of the quantum_queue, and removes this process from
 * the all in waiting_processes list. Returns the removed process.
 */
static process_t *remove_process(schedule_t *waiting_processes,
        list_t *quantum_queue, int quantum_timer) {
    process_t *process;
    waiting_processes->quantum_timer = quantum_timer;
    process = peek(quantum_queue);
    process->quantum = quantum_timer;

    // Removes process from the all waiting processes list
    alter(waiting_processes->all, process,
                    has_same_pid, remove_node);
    dequeue(quantum_queue);
    return process;
}


process_t *schedule_multi(schedule_t *waiting_processes,
        process_t *running_process) {
    assert(waiting_processes!=NULL && waiting_processes->Q1!=NULL);
    waiting_processes->quantum_timer--;
    if (running_process && waiting_processes->quantum_timer < 1) {
    	// Add the previous running process into a lower priority queue
        if (running_process->quantum == QUANTUM_1) {
            enqueue(waiting_processes->all, running_process);
            enqueue(waiting_processes->Q2, running_process);
        } else {
            // Round robin for lowest priority queue
            enqueue(waiting_processes->all, running_process);
            enqueue(waiting_processes->Q3, running_process);
        }
        running_process = NULL;
    }

    // Find highest priority queue that is not empty
    if (!running_process) {
        if (waiting_processes->Q1->head) {
            return remove_process(waiting_processes,
                    waiting_processes->Q1, QUANTUM_1);

        } else if (waiting_processes->Q2->head) {
            return remove_process(waiting_processes,
                    waiting_processes->Q2, QUANTUM_2);

        } else if (waiting_processes->Q3->head) {
            return remove_process(waiting_processes,
                    waiting_processes->Q3, QUANTUM_3);
        }
    }
    return NULL;
}

void free_schedule(schedule_t *schedule) {
    free_list(schedule->all);
    if (schedule->Q1) {
        free_list(schedule->Q1);
        free_list(schedule->Q2);
        free_list(schedule->Q3);
    }
}
