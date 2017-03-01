/*
 * COMP30023 Computer Systems
 * Project 1
 * Author: traillj
 *
 * Process control simulation driver.
 */

#include "process_control.h"
#include "process.h"
#include "linked_list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* The number of columns in the process data file. */
#define NUM_FILE_COL 4


/*
 * Prints usage message.
 */
void usage(void) {
    fprintf(stderr, "Usage: ./simulation -f process_data_file\n");

    fprintf(stderr, "Optional arguments:\n");
    fprintf(stderr, "  -a [fcfs | multi] where\n");
    fprintf(stderr, "    fcfs - first-come first-served queue\n");
    fprintf(stderr, "    multi - multi-level feedback queue (default)\n");

    fprintf(stderr, "  -m memory_size\n");
    fprintf(stderr, "    Defaults to 100\n");
}

/*
 * Reads the file and adds the process data from
 * each line into a queue in order.
 */
list_t *read_process_data_file(char *filename) {
    FILE *fp;
    process_t *process;
    list_t *incoming_processes;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "Cannot open process data file.");
        exit(EXIT_FAILURE);
    }
    incoming_processes = make_empty_list();

    while (1) {
        process = malloc(sizeof(*process));
        assert(process!=NULL);
        if (fscanf(fp, "%d %d %d %d",
                &process->time_created,
                &process->process_id,
                &process->memory_size,
                &process->job_time) != NUM_FILE_COL) {
            free(process);
            break;
        }
        process->address = -1;
        process->time_inserted_in_memory = -1;
        enqueue(incoming_processes, process);
    }
    fclose(fp);
    return incoming_processes;
}

/*
 * Parses command line options, gets the incoming processes
 * and starts the process control simulation.
 */
int main(int argc, char **argv) {
    list_t *incoming_processes;
	char *filename = NULL;
	char scheduling_algorithm = 'm'; // multi
	int memory_size = 100;
	char input;

	// Read in command line options
	while ((input = getopt(argc, argv, "f:a:m:")) != EOF) {
	    switch(input) {
	        case 'f':
	            filename = optarg;
	            break;

	        case 'a':
	            if (strcmp(optarg, "fcfs") == 0) {
	                scheduling_algorithm = 'f';
	            }
	            break;

	        case 'm':
	            memory_size = atoi(optarg);
	            break;

	        default:
	            usage();
	            exit(EXIT_FAILURE);
	    }
	}
	if (!filename) {
	    usage();
	    exit(EXIT_FAILURE);
	}

	incoming_processes = read_process_data_file(filename);
	simulate_process_control(incoming_processes, scheduling_algorithm,
	        memory_size);

	free_list(incoming_processes);
	return 0;
}
