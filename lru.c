#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int counter = 0;


int find_min(){
	int i, min, index;

	min = coremap[0].lru_counter;
	index = 0;

	for(i=1; i<memsize; i++){
		if (coremap[i].lru_counter < min){
			index = i;
			min = coremap[i].lru_counter;
		}
	}

	return index;
}


//algo. Use a global counter
//add a 'counter' field to the frame struct.
//evict the frame in coremap that has the lowest counter field

//on each reference, increment counter.
//on each reference, go to coremap[p->frame].counter and set it to counter


/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
	int idx;
	idx = find_min();
	return idx;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	int i;
	counter++;
	for (i=0; i<memsize; i++){
		if (coremap[i].pte->frame == p->frame){
			coremap[i].lru_counter = counter;
		}
	}
	//coremap[p->frame].lru_counter = counter;
	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
}









