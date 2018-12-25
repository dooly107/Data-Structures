#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int power(int value1, int value2) {
	int loop;
	int ans = 1;
	for (loop = 0; loop < value2; loop++) {
		ans *= value1;
	}
	return ans;
}

int* shell_sort_seq(int Size) {
	int p = 0;
	int q = 0;
	int a;
	int b;
	int c = 0;
	int d = 0;
	int e = 0;
	int f = 0;
	int integer = Size;
	int num_seq = 0;

	int* num_seq_array = NULL;

	if ((Size == 0) || (Size == 1)) {
		return NULL;
	}
	else{
		while (power(2,p) <= integer) {
			p++;
		}
		p--;
		while (power(3,q) <= integer) {
			q++;
		}
		q--;
		for (a = 0; a <= p; a++) {
			for (b = 0; b <= q; b++) {
				if (power(2,a)*power(3,b) <= integer) {
					num_seq++;
				}
			}
		}
		num_seq_array = malloc(sizeof(int) * (num_seq + 1));
		for (c = 0; c <= p; c++) {
			d = 0;
			f = c;
			while ((d <= c) && (power(2,f) * power(3,d) <= integer)) {
				num_seq_array[e] = power(2,f) * power(3,d);
				e++;
				f--;
				d++;
			}

		}
		num_seq_array[e] = '\0';
		return num_seq_array;
	}
}

long* Load_From_File(char* Filename, int* Size) {
	FILE* input = NULL;
	int* size = Size;
	input = fopen(Filename, "rb");

	if (input == NULL) {
		fprintf(stderr, "File cannot be opened");
		fclose(input);
		return NULL;
	}

	fseek(input, 0, SEEK_END);
	*size = ftell(input) / sizeof(long);
	if ((*size) == 0) {
		fprintf(stderr, "Size of the file is zero");
		fclose(input);
		return 0;
	}
	fseek(input, 0, SEEK_SET);

	long* num_arr = malloc(sizeof(long) * (*size));
	if (fread(num_arr, sizeof(long), (*size), input) != (*size)) {
			fprintf(stderr, "Failed to read input file");
	}

	fclose(input);

	return num_arr;
}

int Print_Seq(char* Filename, int Size) {
	int num_loop = 0;
	int num_int = 0;
	FILE* seq = NULL;
	seq = fopen(Filename,"w");
	int* num_seq = shell_sort_seq(Size);
	if ((Size == 0) || (Size == 1)) {
		free(num_seq);
		fclose(seq);
		return 0;
	}
	else {
		while (num_seq[num_int] != '\0') {
			num_int++;
		}
		for(num_loop = 0; num_loop < num_int; num_loop++) {
			fprintf(seq, "%d\n", num_seq[num_loop]);
		}
		free(num_seq);
		fclose(seq);
		return num_int;
	}
}

int Save_To_File(char* Filename, long* Array, int Size) {
	FILE* output = NULL;
	output = fopen(Filename, "wb");
	int success = fwrite(Array, sizeof(long), Size, output);
	fclose(output);
	return success;
}

void Selection_Sort(long* array, int size, double* N_Comp, double* N_Move) {
	int indexmin;
	int temp;
	int i;
	int j;
	for (i = 0; i < size - 1; i++) {
		indexmin = i;
		for(j = i + 1; j < size; j++) {
			(*N_Comp)++;
			if (array[j] < array[indexmin]) {
				indexmin = j;
			}
		}
		temp = array[indexmin];
		array[indexmin] = array[i];
		array[i] = temp;
		(*N_Move) += 3;
	}
}


void Insertion_Sort(long* array, int size, double* N_Comp, double* N_Move) {
	int j;
	int i;
	int temp_num;
	for (j = 1; j < size; j++) {
		temp_num = array[j];
		i = j;
		(*N_Comp)++;
		(*N_Move)++;
		while ((i > 0) && (temp_num < array[i - 1])) {
			if ((array[i - 1] > array[i]) && (i - 1 != 0)) {
				(*N_Comp)++;
			}
			array[i] = array[i - 1];
			(*N_Move)++;
			i--;
		}
		if (i != j) {
			array[i] = temp_num;
			(*N_Move)++;
		}
	}
}

void Shell_Insertion_Sort(long* Array, int Size, double* N_Comp, double* N_Move) {
	int* num_seq = shell_sort_seq(Size);
	int seq_loc = 0;
	int temp_arr_count = 0;
	int temp_arr_loc;
	int adjust_arr_loc = 0;
	double comp_update = 0;
	double move_update = 0;
	int temp_num;


	while (num_seq[seq_loc] != '\0') {
		seq_loc++;
	}
	seq_loc--;

	while (seq_loc != -1) {
		adjust_arr_loc = 0;
		while (adjust_arr_loc < num_seq[seq_loc]) {
			comp_update = 0;
			move_update = 0;
			temp_arr_count = 0;
			while ((adjust_arr_loc + (temp_arr_count * num_seq[seq_loc])) < Size) {
				temp_arr_count++;
			}
			if (temp_arr_count != 1) {
				long* temp_arr = malloc(sizeof(long) * temp_arr_count);
				for (temp_arr_loc = 0; temp_arr_loc < temp_arr_count; temp_arr_loc++) {
					if ((adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])) < Size) {
						temp_arr[temp_arr_loc] = Array[adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])];
					}
				}
				Insertion_Sort(temp_arr, temp_arr_count, &comp_update, &move_update);
				(*N_Comp) += comp_update;
				(*N_Move) += move_update;
				for (temp_arr_loc = 0; temp_arr_loc < temp_arr_count; temp_arr_loc++) {
					if ((adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])) < Size) {
						temp_num = temp_arr[temp_arr_loc];
						Array[adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])] = temp_num;
					}
				}
				free(temp_arr);
			}
			adjust_arr_loc++;
		}
		seq_loc--;
	}
	free(num_seq);
}

void Shell_Selection_Sort(long* Array, int Size, double* N_Comp, double* N_Move) {
	int* num_seq = shell_sort_seq(Size);
	int seq_loc = 0;
	int temp_arr_count = 0;
	int temp_arr_loc;
	int adjust_arr_loc = 0;
	double comp_update = 0;
	double move_update = 0;
	int temp_num;


	while (num_seq[seq_loc] != '\0') {
		seq_loc++;
	}
	seq_loc--;

	while (seq_loc != -1) {
		adjust_arr_loc = 0;
		while (adjust_arr_loc < num_seq[seq_loc]) {
			comp_update = 0;
			move_update = 0;
			temp_arr_count = 0;
			while ((adjust_arr_loc + (temp_arr_count * num_seq[seq_loc])) < Size) {
				temp_arr_count++;
			}
			if (temp_arr_count != 1) {
				long* temp_arr = malloc(sizeof(long) * temp_arr_count);
				for (temp_arr_loc = 0; temp_arr_loc < temp_arr_count; temp_arr_loc++) {
					if ((adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])) < Size) {
						temp_arr[temp_arr_loc] = Array[adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])];
					}
				}
				Selection_Sort(temp_arr, temp_arr_count, &comp_update, &move_update);
				(*N_Comp) += comp_update;
				(*N_Move) += move_update;
				for (temp_arr_loc = 0; temp_arr_loc < temp_arr_count; temp_arr_loc++) {
					if ((adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])) < Size) {
						temp_num = temp_arr[temp_arr_loc];
						Array[adjust_arr_loc + (temp_arr_loc * num_seq[seq_loc])] = temp_num;
					}
				}
				free(temp_arr);
			}
			adjust_arr_loc++;
		}
		seq_loc--;
	}
	free(num_seq);
}
