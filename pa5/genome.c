#include <stdio.h>
#include <stdlib.h>
#include "genome.h"

static int** create_matrix(char* filename, int* sequence, int* integers) {
	FILE* input = NULL;
	int i = 0;
	int j = 0;

	input = fopen(filename, "rb");
	if (input == NULL) {
		fprintf(stderr, "File cannot be opened.");
		fclose(input);
		return NULL;
	}
	fread(sequence, sizeof(int), 1, input);
	fread(integers, sizeof(int), 1, input);

	int** matrix = (int**)malloc(sizeof(int*) * (*integers));
	for(i = 0; i < (*integers); i++){
		matrix[i] = (int*)malloc(sizeof(int) * (*sequence));
	}

	for(i = 0; i < (*integers); i++){
		for(j = 0; j < (*sequence); j++) {
			fread(&matrix[i][j], sizeof(int),1,input);
		}
	}

	fclose(input);
	return matrix;
}

static void free_all(int** matrix, int** adjacent_matrix, int* temp_array, int width, int height) {
	int i = 0;
	for(i = 0; i < height; i++){
		free(matrix[i]);
	}
	free(matrix);

	for(i = 0; i < width; i++){
		free(adjacent_matrix[i]);
	}
	free(adjacent_matrix);
	free(temp_array);
}

static void dfs(int** adjacent_matrix, int* longest_array, int* temp_array, int count, int width, int clear, int pos, int* num_largest){
	 int check_adjacent = 0;
	 int j = 0;
	 int k = 0;
	 for (j = 0; j < width; j++){
   if(adjacent_matrix[count][j] == 0){
     check_adjacent++;
	   }
	 }

  if(check_adjacent == width){
    if(pos > (*num_largest)){
	     (*num_largest) = pos;
	     for(k = 0; k <= pos; k++){
				 longest_array[k] = temp_array[k];
	     }
	  }
	  return;
	}

	for(j = 0; j < width; j++){
	  if(adjacent_matrix[count][j] == 1){
	    if(clear == count){
			pos = 0;
     }
	   temp_array[pos] = count + 1;
     temp_array[pos + 1] = j + 1;
	    pos++;
	    dfs(adjacent_matrix, longest_array, temp_array, j, width, clear, pos, num_largest);
  	}
	}
}


int* Longest_conserved_gene_sequence(char* filename, int* size_of_seq) {
	int width = 0;
	int height = 0;
	int clear = 0;
	int pos = 0;

	int i = 0;
	int j = 0;


	int** matrix = create_matrix(filename, &width, &height);
	int* longest_array = (int*)malloc(sizeof(int) * width);
	int* temp_array = (int*)malloc(sizeof(int) * width);
	int **adjacent_matrix = (int**)malloc(sizeof(int*) * width);
	for(i = 0; i < width; i++){
	  adjacent_matrix[i] = (int*)malloc(sizeof(int) * width);
	}

	for(i = 0; i < width; i++){
	  for(j = 0; j < width; j++){
	    adjacent_matrix[i][j] = 1;
	  }
	}


	int k = 0;
	for(i = 0; i < height; i++){
	  for(j = (width - 1); j >= 0; j--){
	    int temp_var1 = matrix[i][j];
	    for(k = j; k >= 0; k--){
	      int temp_var2 = matrix[i][k];
	      adjacent_matrix[temp_var1 - 1][temp_var2 - 1] = 0;
	    }
	  }
	}

	//Depth First Search
	int count = 0;
	int num_largest = 0;
	for(count = 0; count < width; i++){
		clear = count;
		dfs(adjacent_matrix, longest_array, temp_array, count, width, clear, pos, &num_largest);
	}

	fprintf(stdout,"Length: %d\n", num_largest + 1);

	free_all(matrix, adjacent_matrix, temp_array, width, height);
	return longest_array;
}




/*
#include <stdio.h>
#include <stdlib.h>
#include "genome.h"

static int search_location(Graph** gene_array, int y, int x) {
	int x_coord = 0;
	while(gene_array[y][x_coord].in_order != gene_array[y][x].value) {
		x_coord++;
	}
	return x_coord;
}

static void swap_value(Graph** gene_array, int y, int x, int* sorted_column, int sequence) {
	int x_coord = *sorted_column;
	int temp1 = 0;
	int temp2 = 0;
	while(gene_array[y][x_coord].in_order != gene_array[y][x].value) {
			x_coord++;
	}
	while (y < sequence) {
		temp1 = gene_array[y][x].sort_value;
		gene_array[y][x].sort_value = gene_array[y][x_coord].sort_value;
		gene_array[y][x_coord].sort_value = temp1;

		temp2 = gene_array[y][x].in_order;
		gene_array[y][x].in_order = gene_array[y][x_coord].in_order;
		gene_array[y][x_coord].in_order = temp2;
		y++;
	}
	(*sorted_column)++;
}

static void dfs (adj_list* sort, int value, int* size_of_seq, int num_seq) {
	sort[value].visited = 1;
	adj_list* temp_list = sort[value].next;
	int value_loc = 0;
	while (temp_list != NULL) {
		if (temp_list -> visited == 0) {
			while ((sort[value_loc].adj_value) != (temp_list -> adj_value)) {
				value_loc++;
			}
			dfs(sort, value_loc, size_of_seq, num_seq + 1);
			if ((*size_of_seq) <= num_seq + 1) {
				(*size_of_seq) += num_seq + 1;
			}
		}
		temp_list = temp_list -> next;
	}
	return;
}

static void compare_sort_values(Graph** gene_array, adj_list* sort, int x, int y, int integers, int sequence, int cycle) {
	int xcoord = x;
	int ycoord = y;
	int value = 0;
	if (cycle == integers) {
		return;
	}
	while(xcoord < integers && ycoord < sequence) {
		if (gene_array[ycoord][xcoord].sort_value < gene_array[ycoord][xcoord + cycle].sort_value) {
			ycoord++;
		}
		else {
			break;
		}
	}
	if (ycoord == sequence) {
		while (gene_array[0][xcoord].value != sort[value].adj_value) {
			value++;
		}
		if (sort[value].next == NULL) {
			sort[value].next = (adj_list*)malloc(sizeof(adj_list));
			sort[value].next -> adj_value = gene_array[0][xcoord + cycle].value;
			sort[value].next -> next = NULL;
			sort[value].next -> visited = 0;
		}
		else {
			adj_list* temp_adj = sort[value].next;
			while (temp_adj -> next != NULL) {
				temp_adj = temp_adj -> next;
			}
			temp_adj -> next = (adj_list*)malloc(sizeof(adj_list));
			temp_adj -> next -> adj_value = gene_array[0][xcoord + cycle].value;
			temp_adj -> next -> next = NULL;
			temp_adj -> next -> visited = 0;
		}
	}
	compare_sort_values(gene_array, sort, x, y, integers, sequence, cycle + 1);
}

int* Longest_conserved_gene_sequence(char* filename, int* size_of_seq) {
	FILE* input = NULL;
	int integers = 0;
	int sequence = 0;
	int* gene_seq = NULL;
	Graph** gene_array = NULL;

	input = fopen(filename, "rb");
	if (input == NULL) {
		fprintf(stderr, "File cannot be opened.");
		fclose(input);
		*size_of_seq = 0;
		return NULL;
	}
	fread(&integers, sizeof(int), 1, input);
	fread(&sequence, sizeof(int), 1, input);

	gene_array = (Graph**)malloc(sizeof(Graph*) * sequence);
	int count = 0;
	for (count = 0; count < sequence; count++) {
		gene_array[count] = (Graph*)malloc(sizeof(Graph) * integers);
	}

	int x = 0;
	int y = 0;
	for (y = 0; y < sequence; y++) {
		for (x = 0; x < integers; x++) {
			fread(&(gene_array[y][x].value), sizeof(int), 1, input);
			gene_array[y][x].sort_value = 0;
			gene_array[y][x].in_order = x + 1;
			gene_array[y][x].next = NULL;
		}
	}

	int coord = 0;
	int order = 0;
	for (y = 0; y < sequence; y++) {
		for (x = 0; x < integers; x++) {
			coord = search_location(gene_array, y, x);
			gene_array[y][coord].sort_value = order;
			order++;
		}
		order = 0;
	}

	int sorted_column = 0;
	y = 0;
	for (x = 0; x < integers; x++) {
			swap_value(gene_array, y, x, &sorted_column, sequence);
	}

	adj_list* sort = (adj_list*)malloc(sizeof(adj_list) * sequence);

	for (x = 0; x < integers; x++) {
			sort[x].adj_value = gene_array[0][x].value;
			sort[x].next = NULL;
			sort[x].visited = 0;
	}

	int cycle = 0;
	for (x = 0; x < integers; x++){
		cycle = 1;
		compare_sort_values(gene_array, sort, x, y, integers, sequence, cycle);
	}

	int num_seq = 1;
	for (x = 0; x < integers; x++) {
		num_seq = 0;
		dfs(sort, x, size_of_seq, num_seq);
	}

for (x = 0; x < integers; x++) {
	while(sort[x].next != NULL) {
		adj_list* temp_free = sort[x];
		sort[x] = sort[x].next;
		free(temp_free);
	}
	free(sort[x]);
}

	fclose(input);
	return gene_seq;
}
*/
