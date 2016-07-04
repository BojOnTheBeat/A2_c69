#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

unsigned long *trace_array; //array of vaddrs that we read from the tracefile.

int current_index = 0; //Updated at each reference. (By one)

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

		for (trace_idx=current_index; trace_idx<ref_count; trace_idx++){
			if (trace_array[trace_idx] == (coremap[phys_idx].pte->frame >> PAGE_SHIFT)){
				if(trace_idx > furthest){
					furthest = trace_idx;
					to_evict = phys_idx;
				}
				break; //break the inner loop cuz we've already found the next page reference
			}
		}
	}
	
	return to_evict;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {

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

	//number of vaddrs in tracefile is given by ref_count(in pagetable.h)
	trace_array = malloc(ref_count * sizeof(unsigned long));

	//Open tracefile for reading, as is done in sim.c
	FILE *tfp = stdin;
	if ((tfp = fopen(tracefile, "r")) = NULL){
		perror("Error opening tracefile:");
		exit(1);
	}

	// Read through the trace file and safe the vaddr on each line into
	// the trace_array
	while(fgets(buf, MAXLINE, tfp) != NULL){
		if (buf[0] != '=') {
			sscanf(bufm "%c %lx", &type, &vaddr);
			trace_array[i] = vaddr;
			i++; //**** TODO: Add check to make sure i doesn't get out of bounds?
		}
	}
}

