#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int curr_time = 0;


//Simple helper function to find the minimum timestamp of each frame in physical memory
//The frame with the smallest timestamp is the least recently used frame.
int find_min(){
	int i, min, index;

	min = coremap[0].lru_timestamp;
	index = 0;

	for(i=1; i<memsize; i++){
		if (coremap[i].lru_timestamp < min){
			index = i;
			min = coremap[i].lru_timestamp;
		}
	}

	return index;
}




/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
	int evict_idx;
	evict_idx = find_min(); //Evict the frame in physical memory that has the smallest timestamp.
	return evict_idx;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */

 //Each time a page is accessed the glob_variable curr_time is increased and the frame of the referenced page gets the current_time.
void lru_ref(pgtbl_entry_t *p) {
	int i;
	curr_time++;
	for (i=0; i<memsize; i++){
		if (coremap[i].pte == p){
			coremap[i].lru_timestamp = curr_time;
		}
	}
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
}









