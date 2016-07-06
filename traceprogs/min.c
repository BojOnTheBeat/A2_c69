
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//compare two different arrays and put the minimum of each index into a third array
void compare_min(int *array1, int *array2, int array_size){
	int i;
	int *return_array;
	return_array = malloc(array_size * sizeof(int));

	for(i=0; i<array_size; i++){
		if(array1[i]<= array2[i]){
			return_array[i] = array1[i];
		}else{
			return_array[i] = array2[i];
		}
	}
	free(return_array);
	//return return_array;
}



	
int main(int argc, char ** argv) {
	int array1[10] = {6,4,10,99,54,101,103,2,0,999};
	int array2[10] = {17, 2, 99, 55, 1, 6,100, 8, 202, 8};
	int *result;
	/* Markers used to bound trace regions of interest */
	volatile char MARKER_START, MARKER_END;
	/* Record marker addresses */
	FILE* marker_fp = fopen("min.marker","w");
	if(marker_fp == NULL ) {
		perror("Couldn't open marker file:");
		exit(1);
	}
	fprintf(marker_fp, "%p %p", &MARKER_START, &MARKER_END );
	fclose(marker_fp);

	MARKER_START = 33;
	compare_min(array1, array2, 10);
	//stack_loop(100);
	MARKER_END = 34;

	return 0;
}
