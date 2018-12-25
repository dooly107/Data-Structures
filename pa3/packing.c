#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "packing.h"


static void _reverse_list(List** info_list) {
	List* temp1 = *info_list;
	List* temp2 = *info_list;
	List* temp3 = *info_list;

	temp1 = temp1 -> next -> next;
	temp2 = temp2 -> next;
	temp3 -> next = NULL;
	temp2 -> next = temp3;

	while (temp1 != NULL) {
		temp3 = temp2;
		temp2 = temp1;
		temp1 = temp1 -> next;
		temp2 -> next = temp3;
	}
	*info_list = temp2;
}

static Node* _generate_BST(List* input, int* position) {
	if (input == NULL) {
		return NULL;
	}
	Node* bst_new = NULL;
	List* mod_input = input;
	int loop = *position;
	while (loop != 0) {
		mod_input = mod_input -> next;
		loop--;
	}
	bst_new = (Node*)malloc(sizeof(Node));
	bst_new -> path = mod_input -> path;
	bst_new -> label = mod_input -> label;
	bst_new -> width = mod_input -> width;
	bst_new -> height = mod_input -> height;
	bst_new -> left = NULL;
	bst_new -> right = NULL;
	bst_new -> used = false;
	bst_new -> bin_size_x = 0;
	bst_new -> bin_size_y = 0;
	bst_new -> xcoord = 0;
	bst_new -> ycoord = 0;
	if (bst_new -> path == '\0') {
		return bst_new;
	}
	(*position)++;
	bst_new -> right = _generate_BST(input, position);
	(*position)++;
	bst_new -> left = _generate_BST(input, position);
	return bst_new;
}


Node* Load_BST_From_File(char* Filename) {
	FILE* input = NULL;
	List* get_info = NULL;
	char direction;
	double value;
	int label;
	int size;
	int count = 0;
	List* add_info = NULL;
	input = fopen(Filename, "rt");
	if (input == NULL) {
		fprintf(stderr, "File cannot be opened.");
		fclose(input);
		return NULL;
	}
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	
	
	while (count < size) {
		if (get_info == NULL) {
			get_info = (List*)malloc(sizeof(List));
			direction = fgetc(input);
			if (direction == 'V' || direction == 'H') {
				get_info -> path = direction;
				get_info -> label = '\0';
				get_info -> width = '\0';
				get_info -> height = '\0';
				fseek(input, 1, SEEK_CUR);
				get_info -> next = NULL;
				count = ftell(input);
			}
			else {
				fseek(input, 0, SEEK_SET);
				fscanf(input, "%d", &label);
				get_info -> label = label;
				fseek(input, 1, SEEK_CUR);
				fscanf(input, "%lf", &value);
				get_info -> width = value;
				fseek(input, 1, SEEK_CUR);
				fscanf(input, "%lf", &value);
				get_info -> height = value;
				fseek(input, 2, SEEK_CUR);
				get_info -> path = '\0';
				get_info -> next = NULL;
				count = ftell(input);
			}
		}
		else {
			add_info = get_info;
			while (add_info -> next != NULL) {
				add_info = add_info -> next;
			}
			add_info -> next = (List*)malloc(sizeof(List));
			direction = fgetc(input);
			if (direction == 'V' || direction == 'H') {
				add_info -> next -> path = direction;
				add_info -> next -> label = '\0';
				add_info -> next -> width = '\0';
				add_info -> next -> height = '\0';
				fseek(input, 1, SEEK_CUR);
				add_info -> next -> next = NULL;
				count = ftell(input);
			}
			else {
				fseek(input, -1, SEEK_CUR);
				fscanf(input, "%d", &label);
				add_info -> next -> label = label;
				fseek(input, 1, SEEK_CUR);
				fscanf(input, "%lf", &value);
				add_info -> next -> width = value;
				fseek(input, 1, SEEK_CUR);
				fscanf(input, "%lf", &value);
				add_info -> next -> height = value;
				fseek(input, 2, SEEK_CUR);
				add_info -> next -> path = '\0';
				add_info -> next -> next = NULL;
				count = ftell(input);
			}
		}
	}
	fclose(input);
	_reverse_list(&get_info);
	Node* bst = NULL;
	int position = 0;
	bst = _generate_BST(get_info, &position);
	
	List* list_free = get_info;
	while (list_free != NULL) {
		List* temp = list_free;
		list_free = list_free -> next;
		free(temp);
	}
	return bst;
}

static void free_BST(Node* bst) {
	if (bst == NULL) {
		return;
	}
	else {
		free_BST(bst -> left);
		free_BST(bst -> right);
		free(bst);
	}
}

static void bin_size(Node* bst, double* bin_height, double* bin_width) {
	Node* bst_traverse = bst;
	if (bst_traverse -> path == '\0') {
		return;
	}
	bin_size(bst_traverse -> left, bin_height, bin_width);
	bin_size(bst_traverse -> right, bin_height, bin_width);

	if (bst_traverse -> path == 'V' && bst_traverse -> left -> path == '\0' && bst_traverse -> right -> path == '\0') {
		if (bst_traverse -> left -> height < bst_traverse -> right -> height) {
			*bin_width = bst_traverse -> right -> width + bst_traverse -> left -> width;
			*bin_height = bst_traverse -> right -> height;
			bst -> bin_size_x = *bin_width;
			bst -> bin_size_y = *bin_height;
		}
		else {
			*bin_width = bst_traverse -> right -> width + bst_traverse -> left -> width;
			*bin_height = bst_traverse -> left -> height;
			bst -> bin_size_x = *bin_width;
			bst -> bin_size_y = *bin_height;
		}
	}
	else if (bst_traverse -> path == 'H' && bst_traverse -> left -> path == '\0' && bst_traverse -> right -> path == '\0'){
		if (bst_traverse -> left -> width < bst_traverse -> right -> width) {
			*bin_width = bst_traverse -> right -> width;
			*bin_height = bst_traverse -> right -> height + bst_traverse -> left -> height;
			bst -> bin_size_x = *bin_width;
			bst -> bin_size_y = *bin_height;
		}
		else {
			*bin_width = bst_traverse -> left -> width;
			*bin_height = bst_traverse -> right -> height + bst_traverse -> left -> height;
			bst -> bin_size_x = *bin_width;
			bst -> bin_size_x = *bin_height;
		}
	}
	else if (bst_traverse -> right -> path != '\0' && bst_traverse -> left -> path != '\0') {
		if (bst_traverse -> path == 'H') {
			if (bst_traverse -> left -> bin_size_x < bst_traverse -> right -> bin_size_x) {
				*bin_width = bst_traverse -> right -> bin_size_x;
				*bin_height = bst_traverse -> right -> bin_size_y + bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> left -> bin_size_x;
				*bin_height = bst_traverse -> right -> bin_size_y + bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
		}
		else {
			if (bst_traverse -> left -> bin_size_y < bst_traverse -> right -> bin_size_y) {
				*bin_width = bst_traverse -> right -> bin_size_x + bst_traverse -> left -> bin_size_x;
				*bin_height = bst_traverse -> right -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> right -> bin_size_x + bst_traverse -> left -> bin_size_x;
				*bin_height = bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
		}
	}
	else if (bst_traverse -> path == 'V' && (bst_traverse -> left -> path != '\0' || bst_traverse -> right -> path != '\0')) {
		if (bst_traverse -> left -> path != '\0') {
			if (bst_traverse -> left -> bin_size_y < bst_traverse -> right -> height) {
				*bin_width = bst_traverse -> right -> width + bst_traverse -> left -> bin_size_x;
				*bin_height = bst_traverse -> right -> height;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> right -> width + bst_traverse -> left -> bin_size_x;
				*bin_height = bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;	
			}
		}
		else {
			if (bst_traverse -> right -> bin_size_y < bst_traverse -> left -> height) {
				*bin_width = bst_traverse -> left -> width + bst_traverse -> right -> bin_size_x;
				*bin_height = bst_traverse -> left -> height;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> left -> width + bst_traverse -> right -> bin_size_x;
				*bin_height = bst_traverse -> right -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;	
			}
		}
	}
	else if (bst_traverse -> path == 'H' && (bst_traverse -> left -> path != '\0' || bst_traverse -> right -> path != '\0')) {
		if (bst_traverse -> left -> path != '\0') {
			if (bst_traverse -> left -> bin_size_x < bst_traverse -> right -> width) {
				*bin_width = bst_traverse -> right -> width; 
				*bin_height = bst_traverse -> right -> height + bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> left -> bin_size_x; 
				*bin_height = bst_traverse -> right -> height + bst_traverse -> left -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;	
			}
		}
		else {
			if (bst_traverse -> right -> bin_size_x < bst_traverse -> left -> width) {
				*bin_width = bst_traverse -> left -> width; 
				*bin_height = bst_traverse -> left -> height + bst_traverse -> right -> bin_size_y;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;
			}
			else {
				*bin_width = bst_traverse -> right -> bin_size_x;
				*bin_height = bst_traverse -> right -> bin_size_y + bst_traverse -> left -> height;
				bst -> bin_size_x = *bin_width;
				bst -> bin_size_y = *bin_height;	
			}
		}
	}
}

static void search_coords(Node* pack_bst) {
	Node* coord_bst = pack_bst;
	if (coord_bst -> path == '\0') {
		return;
	}
	else if (coord_bst -> path == 'H') {
		coord_bst -> right -> xcoord = coord_bst -> xcoord;
		coord_bst -> right -> ycoord = coord_bst -> ycoord;
		coord_bst -> left -> xcoord = coord_bst -> xcoord;
		if (coord_bst -> right -> path == '\0') {
			coord_bst -> left -> ycoord = coord_bst -> ycoord + coord_bst -> right -> height;
		}
		else {
			coord_bst -> left -> ycoord = coord_bst -> ycoord + coord_bst -> right -> bin_size_y;
		}
	}
	else if (coord_bst -> path == 'V') {
		coord_bst -> left -> xcoord = coord_bst -> xcoord;
		coord_bst -> left -> ycoord = coord_bst -> ycoord;
		coord_bst -> right -> ycoord = coord_bst -> ycoord;
		if (coord_bst -> left -> path == '\0') {
			coord_bst -> right -> xcoord = coord_bst -> xcoord + coord_bst -> left -> width;
		}
		else {
			coord_bst -> right -> xcoord = coord_bst -> xcoord + coord_bst -> left -> bin_size_x;
		}
	}
	search_coords(pack_bst -> right);
	search_coords(pack_bst -> left);
}

Node* Packing_From_BST(Node* packing) {
	Node* pack_bst = NULL;
	pack_bst = packing;
	double width;
	double height;

	bin_size(pack_bst, &width, &height);
	search_coords(pack_bst);
	return pack_bst;
}


static void save_to_file(Node* packing, FILE* output_file) {
	Node* print = packing;
	if (print == NULL) {
		return;
	}
	save_to_file(print -> left, output_file);
	save_to_file(print -> right, output_file);
	if (print -> path == '\0') {
		fprintf(output_file, "%d %le %le %le %le\n", print -> label, print -> width, print -> height, print -> xcoord, print -> ycoord);
	}
}


static void Find_Rectangle(Node* pack_node, int* largest_index, Node** largest_index_node) {

	if (pack_node == NULL) {
		return;
	}
	else if (pack_node -> label != 'H' && pack_node -> label != 'V' && (pack_node -> label > *largest_index)) {
		*largest_index = pack_node -> label;
		*(largest_index_node) = pack_node;
		return;
	}
	Find_Rectangle(pack_node -> left, largest_index, largest_index_node);
	Find_Rectangle(pack_node -> right, largest_index, largest_index_node);
	return;
}
int Save_Packing_To_File(Node* packing, char* output_file) {
	FILE* output = NULL;
	output = fopen(output_file, "w");
	if (output == NULL) {
		fprintf(stderr, "File cannot be created.");
		fclose(output);
		return 0;
	}
	save_to_file(packing, output);
	int bin_rec = 0;
	Node* bin_rectangle = NULL;

	Find_Rectangle(packing, &bin_rec, &bin_rectangle);
	fprintf(stdout,"Width: %le\n", packing -> bin_size_x);
	fprintf(stdout,"Height: %le\n", packing -> bin_size_y);
	fprintf(stdout,"X-coordinate: %le\n", bin_rectangle -> xcoord);
	fprintf(stdout,"Y-coordinate: %le\n", bin_rectangle -> ycoord);
	free_BST(packing);
	fclose(output);
	return EXIT_SUCCESS;
}
