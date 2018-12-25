#ifndef _GENOME_H_
#define _GENOME_H_
/*
typedef struct _Graph {
	int value;
	int sort_value;
	int in_order;
	struct _adj_list* next;
} Graph;


typedef struct _adj_list {
	int adj_value;
	int visited;
	struct _adj_list* next;
} adj_list;

*/
int* Longest_conserved_gene_sequence(char* filename, int* size_of_seq);

#endif
