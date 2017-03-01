/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Process structure.
 */

#include "process.h"


int has_same_pid(void *process1, void *process2) {
    process_t *p1 = process1;
    process_t *p2 = process2;
    if (p1->process_id == p2->process_id) {
        return 1;
    }
    return 0;
}
