/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Schedule structure and process scheduling functions.
 */

#ifndef SCHEDULING_H_INCLUDED
#define SCHEDULING_H_INCLUDED

#include "linked_list.h"
#include "process.h"

typedef struct {
    list_t *all;       // Contains all processes to be scheduled
    list_t *Q1;        // The queue for high priority processes
    list_t *Q2;        // The queue for medium priority processes
    list_t *Q3;        // The queue for low priority processes
    int quantum_timer; // The time left before a new process starts running
} schedule_t;

/*
 * Allocates space and returns a schedule based
 * on the scheduling algorithm.
 * Quantum related lists are only initialized
 * if required by the algorithm.
 */
schedule_t *initialize_schedule(char scheduling_algorithm);

/*
 * Adds the process to the all queue, and the
 * Q1 queue if this queue has been initialized.
 */
void schedule_process(schedule_t *waiting_processes, process_t *process);

/*
 * Chooses a process to run on the CPU by treating the
 * waiting_processes as a first-come first-served queue.
 * Returns a process if a different process starts running,
 * NULL otherwise.
 */
process_t *schedule_fcfs(schedule_t *waiting_processes,
        process_t *running_process);

/*
 * Chooses a process to run on the CPU by treating the
 * waiting_processes as a multi-level feedback queue.
 * Processes in the lowest priority queue are scheduled
 * with the round robin mechanism.
 * Returns a process if a different process starts running,
 * NULL otherwise.
 */
process_t *schedule_multi(schedule_t *waiting_processes,
        process_t *running_process);

/*
 * Frees the space allocated to the schedule and its lists.
 */
void free_schedule(schedule_t *schedule);

#endif /* SCHEDULING_H_INCLUDED */
