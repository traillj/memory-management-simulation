/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Process control subsystem simulation.
 */

#include "process_control.h"
#include "memory.h"
#include "scheduling.h"
#include "process.h"
#include "linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * Transfers the processes that are to be created according
 * to the input time from the incoming_processes list to the
 * scheduling list waiting_processes.
 */
static void get_arriving_processes(schedule_t *waiting_processes,
        list_t *incoming_processes, int time) {
    process_t *process;
    while (!is_empty(incoming_processes)) {
        process = peek(incoming_processes);
        if (process->time_created <= time) {
            schedule_process(waiting_processes, process);
            dequeue(incoming_processes);
        } else {
            break;
        }
    }
}

/*
 * Prints the process id of the running process, number of holes
 * and processes in memory and the memory usage percentage.
 */
static void print_process_control_info(list_t *memory_holes,
        process_t *running_process, list_t *waiting_processes, int time) {
    printf("time %d, ", time);
    printf("%d running, ", running_process->process_id);
    printf("numprocesses=%d, ", get_num_processes(waiting_processes));
    printf("numholes=%d, ", get_num_holes(memory_holes));
    printf("memusage=%d%%\n", get_memory_usage(memory_holes,
            waiting_processes, running_process));
}

void simulate_process_control(list_t *incoming_processes,
        char scheduling_algorithm, int memory_size) {
    int time = 0;
    schedule_t *waiting_processes;
    list_t *memory_holes;
    process_t *running_process = NULL;
    process_t *new_running_process;

    waiting_processes = initialize_schedule(scheduling_algorithm);
    memory_holes = initialize_memory_holes(memory_size);

    while (1) {
        get_arriving_processes(waiting_processes, incoming_processes, time);

        // If running process finished, free the process and
        // check if simulation is finished or no processes are
        // waiting
        if (running_process && running_process->job_time <= 0) {
            remove_from_memory(memory_holes, running_process);
            free(running_process);
            running_process = NULL;
        }
        if (!running_process && is_empty(waiting_processes->all)) {
            if (is_empty(incoming_processes)) {
                printf("time %d, simulation finished.\n", time);
                break;
            } else {
                time++;
                continue;
            }
        }

        if (scheduling_algorithm == 'f') {
            new_running_process = schedule_fcfs(waiting_processes,
                    running_process);
        } else {
            new_running_process = schedule_multi(waiting_processes,
                                running_process);
        }

        // new_running_process is NULL unless a new process
        // starts running
        if (new_running_process) {
            running_process = new_running_process;
            if (!in_memory(running_process)) {
                insert_in_memory(memory_holes, running_process,
                                    waiting_processes->all, time);
            }
            print_process_control_info(memory_holes, running_process,
                    waiting_processes->all, time);
        }

        running_process->job_time--;
        time++;
    }
    free_list(memory_holes);
    free_schedule(waiting_processes);
}
