/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Process structure.
 */

#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

typedef struct {
    int time_created;
    int process_id;              // Unique
    int memory_size;             // 0 < memory_size <= total memory
    int job_time;
    int address;                 // Valid only when in memory
    int time_inserted_in_memory; // Valid only when in memory
    int quantum;                 // Most recent waiting queue, valid only
                                 // for multilevel feedback queue scheduling
} process_t;

/*
 * For use with alter in linked_list.h as the cmp function.
 * Returns true if the two processes have the same process
 * ID (PID).
 */
int has_same_pid(void *process1, void *process2);

#endif /* PROCESS_H_INCLUDED */
