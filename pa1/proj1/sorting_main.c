#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sorting.h"

int main(int argc, char** argv) {
	clock_t function_start, function_end;
	clock_t sorting_start, sorting_end;
	double io_time = 0;
	double sort_time = 0;
	function_start = clock();
	int Size = 0;
	double N_Comp = 0;
	double N_Move = 0;
	long* array = Load_From_File(argv[2], &Size);
	int num_element;

	if (Size != 0) {
		if (argv[1][0] == 'i') {
			sorting_start = clock();
			Shell_Insertion_Sort(array, Size, &N_Comp, &N_Move);
			sorting_end = clock();
		}
		else if (argv[1][0] == 's') {
			sorting_start = clock();
			Shell_Selection_Sort(array, Size, &N_Comp, &N_Move);
			sorting_end = clock();
		}
		else {
			fprintf(stderr, "Neither Sorting Method is used as argument");
		}

		num_element = Save_To_File(argv[4], array, Size);
		sort_time += difftime(sorting_end, sorting_start);
	}
	else {
		sorting_start = clock();
		sorting_end = clock();
		num_element = 0;
		sort_time = 0;
	}
	int num_int_seq = Print_Seq(argv[3], Size);
	function_end = clock();
	io_time += difftime(function_end, function_start);

	printf("Number of long integers read: %d\n", Size);
	printf("Number of long integers stored: %d\n", num_element);
	printf("Length of sequence: %d\n", num_int_seq);
	printf("Number of comparisons: %le\n", N_Comp);
	printf("Number of Moves: %le\n", N_Move);
	printf("I/O time: %le\n", io_time / CLOCKS_PER_SEC);
	printf("Sorting time: %le\n", sort_time / CLOCKS_PER_SEC);
	free(array);
	return EXIT_SUCCESS;
}
