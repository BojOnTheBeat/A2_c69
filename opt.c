#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h" //added this 
//#include <string.h> //added this


//#define MAXLINE 256//added this
//extern int memsize;
//extern int ref_count;//added this



extern int debug;
//extern char *tracefile;//added this

extern struct frame *coremap;

unsigned long *trace_array; //array of vaddrs that we read from the tracefile.

unsigned long *phys_array; //Basically a copy of coremap. Using coremap directly seems to be breaking stuff

int current_index = 0; //Updated at each reference. (By one)

int line_count = 0;

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {
	//Idea:
	// For frame in coremap
	//   for addr in trace_array
	//		if trace_array[addr] == coremap[frame]
	//		  
	//
	//
	//
	//
	//
	//
	//
	//
	int to_evict;
	int phys_idx, trace_idx;
	int furthest = 0;

	for (phys_idx=0; phys_idx<memsize; phys_idx++){

		for (trace_idx=current_index; trace_idx<line_count; trace_idx++){
			if (trace_array[trace_idx] == phys_array[phys_idx]){
				if(trace_idx > furthest){
					furthest = trace_idx;
					to_evict = phys_idx;
				}
				break; //break the inner loop cuz we've already found the next page reference
			}
		}
		if (trace_idx == line_count){
			return phys_idx;
		}
	}
	
	return to_evict;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {

	int ref_idx = p->frame >> PAGE_SHIFT;

	phys_array[ref_idx] = trace_array[current_index];

	current_index++;

	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {
	char buf[MAXLINE];
	addr_t vaddr = 0;
	char type;
	int i = 0;
	phys_array = malloc(memsize * sizeof(unsigned long));

	//Open tracefile for reading, as is done in sim.c
	FILE *tfp;
	if ((tfp = fopen(tracefile, "r")) == NULL){
		perror("Error opening tracefile:");
		exit(1);
	}

	//Loop through to get the line count.
	while(fgets(buf, MAXLINE, tfp) != NULL){
		line_count++;
	}

	//number of vaddrs in tracefile is given by line_count above.
	trace_array = malloc(line_count * sizeof(unsigned long));

	fclose(tfp);//close the file first

	if ((tfp = fopen(tracefile, "r")) == NULL){ //open it again
		perror("Error opening tracefile:");
		exit(1);
	}


	// Read through the trace file and safe the vaddr on each line into
	// the trace_array
	while(fgets(buf, MAXLINE, tfp) != NULL){
		if (buf[0] != '=') {
			//printf("%i/n", line_count);
			//printf("we're here\n");
			sscanf(buf, "%c %lx", &type, &vaddr);
			trace_array[i] = vaddr;
			i++; //**** TODO: Add check to make sure i doesn't get out of bounds?
		}
	}
	fclose(tfp);
}

