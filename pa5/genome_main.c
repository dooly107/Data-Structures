#include <stdio.h>
#include <stdlib.h>
#include "genome.h"

int main(int argc, char** argv) {
	int size_of_seq = 0;
	int* arr = NULL;

	arr = Longest_conserved_gene_sequence(argv[1], &size_of_seq);
	int i = 0;
	int count = 0;
	while(arr[count] != '\0') {
		count++;
	}
	while(arr[i] != '\0') {
      printf("%d ", arr[i]);
  }
	if (count == 1) {
    printf("1");
  }
	free(arr);
	return EXIT_SUCCESS;
}
