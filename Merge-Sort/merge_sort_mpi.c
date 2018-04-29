#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <mpi.h>

#define SEED 313
#define SIZE 100

typedef struct merge_mpi_data_ {
	int my_rank;
	int comm_sz;
	int *array;
	int size;
} merge_mpi_data;

void merge_print_array(merge_mpi_data *data);
int  *merge_stack(int *left, int left_size, int *right, int right_size);
int  *merge_sort(int *array, int split_pos, int size);
void MPI_merge_sort(merge_mpi_data *data);
void merge_generate_random_array(merge_mpi_data *data, int size, int seed);
merge_mpi_data *merge_init();
void merge_finallize();

int main (void) {
	merge_mpi_data *data;
	data = merge_init();
	merge_generate_random_array(data, SIZE, SEED);
	merge_print_array(data);
	MPI_merge_sort(data);
	merge_print_array(data);
	merge_finallize();
	free(data->array);
	free(data);
	return 0;
}

merge_mpi_data *merge_init() {
	MPI_Init(NULL, NULL);
	merge_mpi_data *ret = malloc(sizeof(merge_mpi_data));
	assert(ret);
	MPI_Comm_size(MPI_COMM_WORLD, &ret->comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &ret->my_rank);
	ret->array = NULL;
	ret->size = 0;
	return ret;
}

void merge_finallize() {
	MPI_Finalize();
	return;
}

void merge_generate_random_array(merge_mpi_data *data, int size, int seed) {
	srand(seed);
	data->array = malloc(sizeof(int) * size);
	assert(data->array);
	data->size = size;
	for (int i = 0; i < size; ++i) {
		data->array[i] = rand() % 1000; // TODO allow bigger numbers
	}
	return;
}

void MPI_merge_sort(merge_mpi_data *data) {
  int *sorted_array = merge_sort(data->array, 0, data->size);
	free(data->array);
	data->array = sorted_array;
}

int *merge_sort(int *array, int l_range, int h_range) {
	if (l_range >= h_range -1) { // Stop recursion
		return merge_stack(array+l_range, 1, NULL, 0); // stack with NULL to avoid code repetion
	}
	int split_pos = (l_range+h_range)/2;
	int *left   = merge_sort(array, l_range, split_pos);
	int *right  = merge_sort(array, split_pos, h_range);
	int *sorted = merge_stack(left, (split_pos - l_range), right, (h_range - split_pos) );
	free(left );
	free(right);
	return sorted;

}
int *merge_stack(int *left, int left_size, int *right, int right_size) {
	int new_array_size = (left_size + right_size);
	int *new_array = malloc(sizeof(int) * new_array_size);
	assert(new_array);
	for (int n_p = 0, r_p = 0, l_p = 0; n_p < new_array_size; ++n_p) {
		if (l_p < left_size) { 
			if (r_p < right_size) {
				if (left[l_p] < right[r_p]) {
					new_array[n_p] = left[l_p];
					++l_p;
				} else {
					new_array[n_p] = right[r_p];
					++r_p;
				}
			} else {
				new_array[n_p] = left[l_p];
				++l_p;
			}
		} else {
			new_array[n_p] = right[r_p];
			++r_p;
		}
	}
	return new_array;
}

void merge_print_array(merge_mpi_data *data) {
	/* if (data->my_rank != 0) */
	/* 	return; */
	printf("Full array:\n");
  printf("[");
	int i;
	for (i = 0; i < data->size - 1; ++i) {
		printf("%i, ", data->array[i]);
	}
	printf( "%i]\n", data->array[data->size - 1]);
	return;
}
