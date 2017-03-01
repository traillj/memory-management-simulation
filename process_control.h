/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Process control subsystem simulation.
 */

#ifndef PROCESS_CONTROL_H_INCLUDED
#define PROCESS_CONTROL_H_INCLUDED

#include "linked_list.h"

/*
 * Simulates the process control subsystem of the kernel.
 * incoming_processes is a list of processes to be run
 * at and for a specified time, each with a PID and
 * memory size.
 * scheduling_algorithm decides how processes are
 * scheduled to run on the CPU.
 * memory_size is the total memory of the system.
 */
void simulate_process_control(list_t *incoming_processes,
        char scheduling_algorithm, int memory_size);

#endif /* PROCESS_CONTROL_H_INCLUDED */
