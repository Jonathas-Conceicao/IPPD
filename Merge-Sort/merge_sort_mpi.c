#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#define SEED 313
#define SIZE 20

typedef struct merge_mpi_data_ {
	int my_rank;
	int comm_sz;
	int *array;
	int size;
	int h_range;
	int l_range;
} merge_mpi_data;

int is_in_list(int key, int *l, int s);
void merge_print_array(merge_mpi_data *data);
char *array_to_string(int *array, int size);
void merge_stack();
void merge_split(int *array, int split_pos, int size);
void merge_sort(merge_mpi_data *data);
void merge_generate_random_array(merge_mpi_data *data, int seed, int size);
merge_mpi_data *merge_init();
void merge_finallize();

int main (void) {
	merge_mpi_data *data;
	data = merge_init();
  merge_generate_random_array(data, SEED, SIZE);
	merge_print_array(data);
	merge_sort(data);
	merge_finallize();
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

void merge_generate_random_array(merge_mpi_data *data, int seed, int size) {
	srand(seed);
	data->array = malloc(sizeof(int) * size);
	assert(data->array);
	data->size = size;
	for (int i = 0; i < size; ++i) {
		data->array[i] = rand() % 1000; // TODO: Eu tenho que remover isso aqui antes de entregar
	}
	return;
}

void merge_sort(merge_mpi_data *data) {
	if (data->my_rank == 0) {
		int l_range;
		int h_range;
		int step = data->size / data->comm_sz;
		for (int i = 1; i < data->comm_sz; ++i) { // Skips 0 so it doesn't send a message to itself
			l_range = step*i;
			h_range = step*(i+1);
			MPI_Send(&l_range, 1, MPI_INT, i, 0, MPI_COMM_WORLD); // Sends lower bound
			MPI_Send(&h_range, 1, MPI_INT, i, 1, MPI_COMM_WORLD); // Sends upper bound
		}
		data->l_range = 0;
		data->h_range = step;
	} else {
		MPI_Recv(&data->l_range, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&data->h_range, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	printf("For %i, l_range: %i, h_range: %i\n", data->my_rank, data->l_range, data->h_range);
	merge_split(data->array, data->l_range, data->h_range);
}

void merge_split(int *array, int l_range, int h_range) {
	printf("Spliting:\n");
	int split_pos = (l_range+h_range)/2;
	char *h1 = array_to_string(array+l_range, split_pos - l_range);
	char *h2 = array_to_string(array+split_pos, h_range - split_pos);
	printf("h1: %s\nh2: %s\n", h1, h2);
	free(h1);
	free(h2);
}

void merge_stack() {
	
}

char *array_to_string(int *array, int size) {
	char *s = malloc( sizeof(char) * (3+(6*size)) );
	assert(s);
	sprintf(s, "[");
	for (int i = 0; i < size-1; ++i) {
		sprintf(s, "%s%i, ", s, array[i]);
	}
	sprintf(s, "%s%i]", s, array[size-1]);
	return s;
}

void merge_print_array(merge_mpi_data *data) {
	if (data->my_rank != 0)
		return;
	printf("Full array:\n");
  char *s = array_to_string(data->array, data->size);
	printf("%s\n", s);
	free(s);
	return;
}

int is_in_list(int key, int *l, int s) {
	for (int i = 0; i < s; i++)
		if (l[i] == key)
			return 1;
	return 0;
}
