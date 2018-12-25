#include <stdlib.h>
#include <stdio.h>
#include "river.h"

int main(int argc, char** argv) {
    Node** input = NULL;
    int num_row = 0;
    int num_col = 0;
    int count_r = 0;
    int count_c = 0;
    int min_rot = '\0';
    int end_row = 0;
    input = Load_From_File(argv[1], &num_row, &num_col);
    num_rotation_count(input, num_row, num_col);
    for (count_r = 0; count_r < num_row; count_r++) {
      for (count_c = 0; count_c < num_col; count_c++) {
        update_rotation_count(input, num_row, num_col, count_r, count_c, &min_rot, &end_row);
      }
    }
    free_matrix(input, num_row, num_col);
    if (min_rot > 20) {
      min_rot -= 1;
    }
    printf("%d\n", min_rot + 1);
    return EXIT_SUCCESS;
}
