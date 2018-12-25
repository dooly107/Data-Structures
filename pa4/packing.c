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
	bst_new -> new_width = mod_input -> width;
	bst_new -> new_height = mod_input -> height;
	bst_new -> new_path = mod_input -> path;
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

static void bin_size(Node* bst) {
	if (bst -> left == NULL) {
		return;
	}
	if (bst -> left -> width == 0) {
		bin_size(bst -> left);
	}
	if (bst -> right -> width == 0) {
		bin_size(bst -> right);
	}
	if (bst -> path == 'H') {
		bst -> height = bst -> left -> height + bst -> right -> height;
		if (bst -> left -> width < bst -> right -> width) {
			bst -> width = bst -> right -> width;
		}
		else {
			bst -> width = bst -> left -> width;
		}
	}
	else if (bst -> path == 'V') {
		bst -> width = bst -> left -> width + bst -> right -> width;
		if (bst -> left -> height < bst -> right -> height) {
			bst -> height = bst -> right -> height;
		}
		else {
			bst -> height = bst -> left -> height;
		}
	}
	return;
}

static void search_coords(Node* pack_bst) {
	Node* coord_bst = pack_bst;
	if (coord_bst == NULL) {
		return;
	}
	if (coord_bst -> path == 'H') {
		coord_bst -> right -> xcoord = coord_bst -> xcoord;
		coord_bst -> right -> ycoord = coord_bst -> ycoord;
		coord_bst -> left -> xcoord = coord_bst -> xcoord;
		coord_bst -> left -> ycoord = coord_bst -> ycoord + coord_bst -> right -> height;
	}
	else if (coord_bst -> path == 'V') {
		coord_bst -> left -> xcoord = coord_bst -> xcoord;
		coord_bst -> left -> ycoord = coord_bst -> ycoord;
		coord_bst -> right -> ycoord = coord_bst -> ycoord;
		coord_bst -> right -> xcoord = coord_bst -> xcoord + coord_bst -> left -> width;
	}
	search_coords(pack_bst -> left);
	search_coords(pack_bst -> right);
}

Node* Packing_From_BST(Node* packing) {
	Node* pack_bst = NULL;
	pack_bst = packing;

	bin_size(pack_bst);
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

static void compare_wh(Node* pack_bst, double compw, double comph, double neww, double newh, double* best_width, double* best_height) {
	double orig_area = 0;
	double new_area = 0;
	double best_area = 0;
	orig_area = compw * comph;
	new_area = neww * newh;
	best_area = (*best_width) * (*best_height);
	if (best_area == 0) {
		if (((new_area <= orig_area) || ((new_area == orig_area) && (neww < compw))) == 1) {
			pack_bst -> new_width = neww;
			pack_bst -> new_height = newh;
			*best_width = neww;
			*best_height = newh;
		}
		else {
			pack_bst -> new_width = compw;
			pack_bst -> new_height = comph;
			*best_width = compw;
			*best_height = comph;
		}
	}
	else {
		if ((new_area < orig_area) && (new_area < best_area)) {
			pack_bst -> new_width = neww;
			pack_bst -> new_height = newh;
			*best_width = neww;
			*best_height = newh;
		}
		else {
			pack_bst -> new_width = compw;
			pack_bst -> new_height = comph;
		}
	}
}

static void decide_new_wh(char path, double* new_w, double* new_h, double tempw1, double temph1, double tempw2, double temph2) {
	if (path == 'H') {
		if (tempw1 > tempw2) {
			*new_w = tempw1;
		}
		else {
			*new_w = tempw2;
		}
		*new_h = temph1 + temph2;
	}
	else {
		if (temph1 > temph2) {
			*new_h = temph1;
		}
		else {
			*new_h = temph2;
		}
		*new_w = tempw1 + tempw2;
	}
}


static void re_root(Node* pack_bst, Node* top_root, double orig_width, double orig_height, double temp_width, double temp_height, double* best_width, double* best_height) {
	Node* root = pack_bst;
	double temp_width1 = temp_width;
	double temp_height1 = temp_height;
	double temp_width2 = 0;
	double temp_height2 = 0;
	double temp_width3 = temp_width;
	double temp_height3 = temp_height;
	double new_w = 0;
	double new_h = 0;
	if (root -> path == '\0') {
		return;
	}
	//root left
	if (root -> left -> path != '\0') {
		if (root -> path == 'H') {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> right -> width < root -> right -> width) {
					temp_width1 = root -> right -> width;
				}
				else {
					temp_width1 = root -> left -> right -> width;
				}
				temp_height1 = root -> right -> height + root -> left -> right -> height;
			}
			else {
				if (temp_width1 < root -> left -> right -> width) {
					temp_width3 = temp_width1;
					temp_width1 = root -> left -> right -> width;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_height3 = temp_height1;
				temp_height1 = temp_height1 + root -> left -> right -> height;
			}
		}
		else {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> right -> height < root -> right -> height) {
					temp_height1 = root -> right -> height;
				}
				else {
					temp_height1 = root -> left -> right -> height;
				}
				temp_width1 = root -> right -> width + root -> left -> right -> width;
			}
			else {
				if (temp_height1 < root -> left -> right -> height) {
					temp_height3 = temp_height1;
					temp_height1 = root -> left -> right -> height;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_width3 = temp_width1;
				temp_width1 = temp_width1 + root -> left -> right -> width;
			}
		}
		temp_width2= root -> left -> left -> width;
		temp_height2= root -> left -> left -> height;
		decide_new_wh(root -> left -> path, &new_w, &new_h, temp_width1, temp_height1, temp_width2, temp_height2);
		compare_wh(top_root, orig_width, orig_height, new_w, new_h, best_width, best_height);

		re_root(pack_bst -> left, top_root, orig_width, orig_height, temp_width1, temp_height1, best_width, best_height);

		if (pack_bst == top_root) {
			temp_width1 = 0;
			temp_height1 = 0;
		}
		else {
			temp_width1 = temp_width3;
			temp_height1 = temp_height3;
		}
		
		if (root -> path == 'H') {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> left -> width < root -> right -> width) {
					temp_width1 = root -> right -> width;
				}
				else {
					temp_width1 = root -> left -> left -> width;
				}
				temp_height1 = root -> right -> height + root -> left -> left -> height;
			}
			else {
				if (temp_width1 < root -> left -> left -> width) {
					temp_width3 = temp_width1;
					temp_width1 = root -> left -> left -> width;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_height3 = temp_height1;
				temp_height1 = temp_height1 + root -> left -> left -> height;
			}
		}
		else {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> left -> height < root -> right -> height) {
					temp_height1 = root -> right -> height;
				}
				else {
					temp_height1 = root -> left -> left -> height;
				}
				temp_width1 = root -> right -> width + root -> left -> left -> width;
			}
			else {
				if (temp_height1 < root -> left -> left -> height) {
					temp_height3 = temp_height1;
					temp_height1 = root -> left -> left -> height;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_width3 = temp_width1;
				temp_width1 = temp_width1 + root -> left -> left -> width;
			}
		}
		temp_width2 = root -> left -> right -> width;
		temp_height2 = root -> left -> right -> height;
		decide_new_wh(root -> left -> path, &new_w, &new_h, temp_width1, temp_height1, temp_width2, temp_height2);
		compare_wh(top_root, orig_width, orig_height, new_w, new_h, best_width, best_height);
	}


	if (pack_bst == top_root) {
		temp_width1 = 0;
		temp_height1 = 0;
	}
	else {
		temp_width1 = temp_width3;
		temp_height1 = temp_height3;
	}

	//root right
	if (root -> right -> path != '\0') {
		if (root -> path == 'H') {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> width < root -> right -> right -> width) {
					temp_width1 = root -> right -> right -> width;
				}
				else {
					temp_width1 = root -> left -> width;
				}
				temp_height1 = root -> left -> height + root -> right -> right -> height;
			}
			else {
				if (temp_width1 < root -> right -> right -> width) {
					temp_width3 = temp_width1;
					temp_width1 = root -> right -> right -> width;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_height3 = temp_height1;
				temp_height1 = temp_height1 + root -> right -> right -> height;
			}
		}
		else {
			if (temp_width1 == 0 && temp_height1 == 0) {
				if (root -> left -> height < root -> right -> right -> height) {
					temp_height1 = root -> right -> right -> height;
				}
				else {
					temp_height1 = root -> left -> height;
				}
				temp_width1 = root -> left -> width + root -> right -> right -> height;
			}
			else {
				if (temp_height1 < root -> right -> right -> height) {
					temp_height3 = temp_height1;
					temp_height1 = root -> right -> right -> height;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_width3 = temp_width1;
				temp_width1 = temp_width1 + root -> right -> right -> width;
			}
		}
		temp_width2 = root -> right -> left -> width;
		temp_height2 = root -> right -> left -> height;
		decide_new_wh(root -> right -> path, &new_w, &new_h, temp_width1, temp_height1, temp_width2, temp_height2);
		compare_wh(top_root, orig_width, orig_height, new_w, new_h, best_width, best_height);

		re_root(pack_bst -> right, top_root, orig_width, orig_height, temp_width1, temp_height1, best_width, best_height);

		if (pack_bst == top_root) {
			temp_width1 = 0;
			temp_height1 = 0;
		}
		else {
			temp_width1 = temp_width3;
			temp_height1 = temp_height3;
		}

		if (root -> path == 'H') {
			if (temp_width1 == 0 && temp_height == 0) {
				if (root -> left -> width < root -> right -> left -> width) {
					temp_width1 = root -> right -> left -> width;
				}
				else {
					temp_width1 = root -> left -> width;
				}
				temp_height1 = root -> left -> height + root -> right -> left -> height;
			}
			else {
				if (temp_width1 < root -> right -> left -> width) {
					temp_width3 = temp_width1;
					temp_width1 = root -> right -> left -> width;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_height3 = temp_height1;
				temp_height1 = temp_height1 + root -> right -> left -> height;
			}
		}
		else {
			if (temp_width1 == 0 && temp_height == 0) {
				if (root -> left -> height < root -> right -> left -> height) {
					temp_height1 = root -> right -> left -> height;
				}
				else {
					temp_height1 = root -> left -> height;
				}
				temp_width1 = root -> left -> width + root -> right -> left -> height;
			}
			else {
				if (temp_height1 < root -> right -> left -> height) {
					temp_height3 = temp_height1;
					temp_height1 = root -> right -> left -> height;
				}
				else {
					temp_width3 = temp_width1;
				}
				temp_width3 = temp_width1;
				temp_width1 = temp_width1 + root -> right -> left -> width;
			}
		}
		temp_width2 = root -> right -> right -> width;
		temp_height2 = root -> right -> right -> height;
		decide_new_wh(root -> right -> path, &new_w, &new_h, temp_width1, temp_height1, temp_width2, temp_height2);
		compare_wh(top_root, orig_width, orig_height, new_w, new_h, best_width, best_height);
	}
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

	double width = packing -> width;
	double height = packing -> height;
	double temp_w = 0;
	double temp_h = 0;
	double best_w = 0;
	double best_h = 0;
	re_root(packing, packing, width, height, temp_w, temp_h, &best_w, &best_h);
	fprintf(stdout,"Width: %le\n", packing -> width);
	fprintf(stdout,"Height: %le\n", packing -> height);
	fprintf(stdout,"X-coordinate: %le\n", bin_rectangle -> xcoord);
	fprintf(stdout,"Y-coordinate: %le\n", bin_rectangle -> ycoord);
	fprintf(stdout,"Best width: %le\n", best_w);
	fprintf(stdout,"Best height: %le\n", best_h);
	free_BST(packing);
	fclose(output);
	return EXIT_SUCCESS;
}

