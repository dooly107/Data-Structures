#ifndef _PACKING_H_
#define _PACKING_H_
#include <stdbool.h>


typedef struct _Node {
	char   path;
	int    label;
	double width;
	double height;
	struct _Node* left;
	struct _Node* right;
	bool   used;
	double xcoord;
	double ycoord;
	double bin_size_x;
	double bin_size_y;
} Node;

typedef struct _List {
	char path;
	int label;
	double width;
	double height;
	struct _List* next;
} List;

typedef struct _Size {
	double xcoord;
	double ycoord;
} Size;


Node* Load_BST_From_File(char* Filename);
Node* Packing_From_BST(Node* pack_bst);
int Save_Packing_To_File(Node* packing, char* output_file);

#endif

