#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int clock[memsize];
int hand = 0;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {

	while(clock[hand]!=0){

		clock[hand] = 0;
		hand++;
		hand = hand % memsize;
	}
	
	return hand;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	clock[p->frame >> PAGE_SHIFT] = 1; //get the frame (in regular ints)

	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {
	int i = 0;
	for (i=0; i<memsize; i++){
		clock[i] = 0;
	}
}
