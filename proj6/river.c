#include <stdio.h>
#include <stdlib.h>
#include "river.h"

Node** Load_From_File(char* filename, int* num_row, int* num_col) {
  FILE* input = NULL;
  input = fopen(filename, "r");
  if (input == NULL) {
    fprintf(stderr, "File cannot be opened.");
    fclose(input);
    return NULL;
  }

  int row = 0;
  int column = 0;
  char val = '\0';
  int input_val = 0;
  fscanf(input, "%i", &input_val);
  fseek(input, 1, SEEK_CUR);
  row = input_val;
  *num_row = row - 1;
  fscanf(input, "%i", &input_val);
  fseek(input, 1, SEEK_CUR);
  column = input_val;
  *num_col = column;


  int count_row = 0;
  Node** pole_matrix = (Node**)malloc(sizeof(Node*) * (row - 1));
  for(count_row = 0; count_row < row - 1; count_row++) {
    pole_matrix[count_row] = (Node*)malloc(sizeof(Node) * column);
  }

  int count_col = 0;
  for(count_row = 0; count_row < row - 1; count_row++) {
    fread(&val, sizeof(char), 1, input);
    if (val == '\n') {
    }
    else {
      fseek(input, -1, SEEK_CUR);
    }
    for(count_col = 0; count_col < column; count_col++) {
      fscanf(input, "%c", &val);
      pole_matrix[count_row][count_col].value = val - '0';
      pole_matrix[count_row][count_col].num_rotation = 0;
      pole_matrix[count_row][count_col].updated = 0;
      pole_matrix[count_row][count_col].prev_num_rotation = 0;
    }
  }
  fclose(input);
  return pole_matrix;
}

void num_rotation_count (Node** pole_matrix, int row, int column) {
  int count_row = 0;
  int count_column = 0;
  int num_rot = 0;
  for (count_row = 0; count_row < row; count_row++) {
    num_rot = 0;
    for (count_column = 0; count_column < column; count_column++) {
      if (count_column == 0) {
        if (pole_matrix[count_row][count_column].value == 0) {
          num_rot += 1;
          pole_matrix[count_row][count_column].num_rotation = num_rot;
          pole_matrix[count_row][count_column].prev_num_rotation = num_rot;
        }
        if (pole_matrix[count_row][count_column].value == 1) {
          num_rot += 0;
          pole_matrix[count_row][count_column].num_rotation = num_rot;
          pole_matrix[count_row][count_column].prev_num_rotation = num_rot;
        }
      }
      else {
        if (pole_matrix[count_row][count_column].value == 0) {
          num_rot += 2;
          pole_matrix[count_row][count_column].num_rotation = num_rot;
          pole_matrix[count_row][count_column].prev_num_rotation = num_rot;
        }
        if (pole_matrix[count_row][count_column].value == 1) {
          num_rot += 1;
          pole_matrix[count_row][count_column].num_rotation = num_rot;
          pole_matrix[count_row][count_column].prev_num_rotation = num_rot;
        }
      }
    }
  }
}

void retrieve_minimum_val(Node** pole_matrix, int row, int column, int* min_rot, int* end_row) {
  int j = 0;
  for (j = 0; j < row; j++) {
    if ((*min_rot) == '\0') {
      (*min_rot) = pole_matrix[j][column - 1].num_rotation;
      (*end_row) = j;
    }
    else if ((*min_rot) > pole_matrix[j][column - 1].num_rotation) {
      (*min_rot) = pole_matrix[j][column - 1].num_rotation;
      (*end_row) = j;
    }
  }
}

void row_count_update (Node** pole_matrix, int row, int column, int cur_row, int cur_col) {
  int i = 0;
  for (i = cur_col + 1; i < column; i++) {
    if (pole_matrix[cur_row][i].value == 0) {
      if (cur_col == column - 1) {
        if (pole_matrix[cur_row][i].prev_num_rotation > pole_matrix[cur_row][i].num_rotation) {
          pole_matrix[cur_row][i].prev_num_rotation = pole_matrix[cur_row][i].num_rotation;
        }
        pole_matrix[cur_row][i].num_rotation = pole_matrix[cur_row][i - 1].num_rotation + 1;
      }
      else {
        if (pole_matrix[cur_row][i].prev_num_rotation > pole_matrix[cur_row][i].num_rotation) {
          pole_matrix[cur_row][i].prev_num_rotation = pole_matrix[cur_row][i].num_rotation;
        }
        pole_matrix[cur_row][i].num_rotation = pole_matrix[cur_row][i - 1].num_rotation + 2;
      }
    }
    if (pole_matrix[cur_row][i].value == 1) {
      if (pole_matrix[cur_row][i].prev_num_rotation > pole_matrix[cur_row][i].num_rotation) {
        pole_matrix[cur_row][i].prev_num_rotation = pole_matrix[cur_row][i].num_rotation;
      }
      pole_matrix[cur_row][i].num_rotation = pole_matrix[cur_row][i - 1].num_rotation + 1;
    }
  }
}

void update_rotation_count (Node** pole_matrix, int row, int column, int count_row, int count_column, int* min_rot, int* end_row) {
  int update = 0;
  int prev_row = 0;
  int prev_col = 0;
  //NORTH
  if ((count_row - 1 >= 0) && (pole_matrix[count_row - 1][count_column].value == 1) && (pole_matrix[count_row - 1][count_column].updated != 1)  && (pole_matrix[count_row - 1][count_column].num_rotation > pole_matrix[count_row][count_column].num_rotation)) {
    pole_matrix[count_row - 1][count_column].prev_num_rotation = pole_matrix[count_row - 1][count_column].num_rotation;
    pole_matrix[count_row - 1][count_column].num_rotation = pole_matrix[count_row][count_column].num_rotation;
    pole_matrix[count_row - 1][count_column].updated = 1;
    row_count_update(pole_matrix, row, column, count_row - 1, count_column);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row - 1;
    update = 1;
  }
  //NORTH_EAST
  else if ((count_row - 1 >= 0) && (count_column + 1 < column) && (pole_matrix[count_row - 1][count_column + 1].value == 1) && (pole_matrix[count_row - 1][count_column + 1].updated != 1) && ((pole_matrix[count_row - 1][count_column + 1].num_rotation) - (pole_matrix[count_row][count_column].num_rotation) >= 2)) {
    pole_matrix[count_row - 1][count_column + 1].prev_num_rotation = pole_matrix[count_row - 1][count_column + 1].num_rotation;
    pole_matrix[count_row - 1][count_column + 1].num_rotation = pole_matrix[count_row][count_column].num_rotation + 1;
    pole_matrix[count_row - 1][count_column + 1].updated = 1;
    row_count_update(pole_matrix, row, column, count_row - 1, count_column + 1);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row - 1;
    count_column = count_column + 1;
    update = 1;
  }
  //SOUTH_EAST
  else if ((count_column + 1 < column) && (count_row + 1 < row) && (pole_matrix[count_row + 1][count_column + 1].updated != 1) && (pole_matrix[count_row + 1][count_column + 1].value == 1) && ((pole_matrix[count_row + 1][count_column + 1].num_rotation) - (pole_matrix[count_row][count_column].num_rotation) >= 2)) {
    pole_matrix[count_row + 1][count_column + 1].prev_num_rotation = pole_matrix[count_row + 1][count_column + 1].num_rotation;
    pole_matrix[count_row + 1][count_column + 1].num_rotation = pole_matrix[count_row][count_column].num_rotation + 1;
    pole_matrix[count_row + 1][count_column + 1].updated = 1;
    row_count_update(pole_matrix, row, column, count_row + 1, count_column + 1);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row + 1;
    count_column = count_column + 1;
    update = 1;
  }
  //SOUTH
  else if ((count_row + 1 < row) && (pole_matrix[count_row + 1][count_column].value == 1) && (pole_matrix[count_row + 1][count_column].updated != 1) && (pole_matrix[count_row + 1][count_column].num_rotation > pole_matrix[count_row][count_column].num_rotation)) {
    pole_matrix[count_row + 1][count_column].prev_num_rotation = pole_matrix[count_row + 1][count_column].num_rotation;
    pole_matrix[count_row + 1][count_column].num_rotation = pole_matrix[count_row][count_column].num_rotation;
    pole_matrix[count_row + 1][count_column].value = 1;
    row_count_update(pole_matrix, row, column, count_row + 1, count_column);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row + 1;
    update = 1;
  }
  //SOUTH_WEST
  else if ((count_row + 1 < row) && (count_column - 1 < column) && (pole_matrix[count_row + 1][count_column - 1].value == 1) && (pole_matrix[count_row + 1][count_column - 1].updated != 1) && ((pole_matrix[count_row + 1][count_column - 1].num_rotation) - (pole_matrix[count_row][count_column].num_rotation) >= 2)) {
    pole_matrix[count_row + 1][count_column - 1].prev_num_rotation = pole_matrix[count_row + 1][count_column - 1].num_rotation;
    pole_matrix[count_row + 1][count_column - 1].num_rotation = pole_matrix[count_row][count_column].num_rotation + 1;
    pole_matrix[count_row + 1][count_column - 1].updated = 1;
    row_count_update(pole_matrix, row, column, count_row + 1, count_column - 1);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row + 1;
    count_column = count_column - 1;
    update = 1;
  }
  //WEST
  else if ((count_column - 1 >= 0) && (pole_matrix[count_row][count_column - 1].value == 1) && (pole_matrix[count_row][count_column - 1].updated != 1) && (pole_matrix[count_row][count_column - 1].num_rotation > pole_matrix[count_row][count_column].num_rotation)) {
    pole_matrix[count_row][count_column - 1].prev_num_rotation = pole_matrix[count_row][count_column - 1].num_rotation;
    pole_matrix[count_row][count_column - 1].num_rotation = pole_matrix[count_row][count_column].num_rotation + 1;
    pole_matrix[count_row][count_column - 1].updated = 1;
    row_count_update(pole_matrix, row, column, count_row, count_column - 1);
    prev_row = count_row;
    prev_col = count_column;
    count_column = count_column - 1;
    update = 1;
  }
  //NORTH_WEST
  else if ((count_column - 1 >= 0) && (count_row - 1 >= 0) && (pole_matrix[count_row - 1][count_column - 1].updated != 1) && (pole_matrix[count_row - 1][count_column - 1].value == 1) && ((pole_matrix[count_row - 1][count_column - 1].num_rotation) - (pole_matrix[count_row][count_column].num_rotation) >= 2)) {
    pole_matrix[count_row - 1][count_column - 1].prev_num_rotation = pole_matrix[count_row - 1][count_column - 1].num_rotation;
    pole_matrix[count_row - 1][count_column - 1].num_rotation = pole_matrix[count_row][count_column].num_rotation + 1;
    pole_matrix[count_row - 1][count_column - 1].updated = 1;
    row_count_update(pole_matrix, row, column, count_row - 1, count_column - 1);
    prev_row = count_row;
    prev_col = count_column;
    count_row = count_row - 1;
    count_column = count_column - 1;
    update = 1;
  }
  if (update == 1) {
    update_rotation_count(pole_matrix, row, column, count_row, count_column, min_rot, end_row);
    retrieve_minimum_val(pole_matrix, row, column, min_rot, end_row);
    pole_matrix[prev_row][prev_col].updated = 0;
  }
  else if (update == 0) {
    row_count_update(pole_matrix, row, column, count_row, count_column);
    retrieve_minimum_val(pole_matrix, row, column, min_rot, end_row);
  }
}


void free_matrix (Node** pole_matrix, int row, int column) {
  int i = 0;
  for (i = 0; i < row; i++) {
      free(pole_matrix[i]);
  }
  free(pole_matrix);
}
