#ifndef RIVER_H
#define RIVER_H


typedef struct _node {
	int num_rotation;
	int value;
	int updated;
	int prev_num_rotation;
} Node;

Node** Load_From_File(char* filename, int* num_row, int* num_col);
void update_rotation_count(Node** pole_matrix, int row, int column, int count_row, int count_column, int* min_rot, int* end_row);
void num_rotation_count(Node** pole_matrix, int row, int column);
void retrieve_minimum_val(Node** pole_matrix, int row, int column, int* min_rot, int* end_row);
void free_matrix (Node** pole_matrix, int row, int column);

#endif
