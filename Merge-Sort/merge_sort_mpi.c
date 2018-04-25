#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#define SEED 313
#define SIZE 10

typedef struct merge_mpi_data_ {
	int my_rank;
	int comm_sz;
	int *array;
	int size;
} merge_mpi_data;


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

void merge_generate_random_array(merge_mpi_data *data, int seed, int size) {
	srand(seed);
	data->array = malloc(sizeof(int) * size);
	assert(data->array);
	data->size = size;
	for (int i = 0; i < size; ++i) {
		data->array[i] = rand();
	}
	return;
}

int merge_sort() {
	
}

void merge_print_array(merge_mpi_data *data) {
	if (data->my_rank != 0)
		return;
	printf("Priting array:\n");
	for (int i = 0; i < data->size; ++i) {
		printf("array[%i] = %i\n", i, data->array[i]);
	}
	return;
}

int main (void) {
	merge_mpi_data *data;
	data = merge_init();
  merge_generate_random_array(data, SEED, SIZE);
	merge_print_array(data);
	merge_finallize();
	return 0;
}
