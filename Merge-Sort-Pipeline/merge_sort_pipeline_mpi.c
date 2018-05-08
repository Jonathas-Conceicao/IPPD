#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#define assert__(x, m) for ( ; !(x) ; assert(x) ) fprintf(stderr,"ASSERT ERROR: %s\n", m);

typedef struct merge_mpi_data_ {
	int my_rank;
	int comm_sz;
	int *array;
	int size;
	long double process_time;
	long double copy_time;
} merge_mpi_data;

void merge_print_array(merge_mpi_data *data);
void merge_tell_times(merge_mpi_data *data);
int  *merge_stack(int *left, int left_size, int *right, int right_size);
int  *merge_sort(int *array, int split_pos, int size);
void MPI_merge_sort(merge_mpi_data *data);
void merge_generate_random_array(merge_mpi_data *data, int size, int seed);
merge_mpi_data *merge_init();
void merge_finallize();
int sorted(int *array, int size);

int main(int argc, char *argv[]) {
	assert__(argc == 3, "ASSERT ERROR: Invalid number of arguments!\nExpected array size and random seed as arg 1 and 2.");
	int SIZE, SEED, ret;
	sscanf(argv[1], "%i", &SIZE);
	sscanf(argv[2], "%i", &SEED);
	merge_mpi_data *data;
	long double total_time;
	data = merge_init();
	merge_generate_random_array(data, SIZE, SEED);
	/* merge_print_array(data); */
	if (data->my_rank == 0)
		total_time = MPI_Wtime();
	MPI_merge_sort(data);
	merge_tell_times(data);
	MPI_Barrier(MPI_COMM_WORLD);
	if (data->my_rank == 0)
		printf("%i, %Lf\n", data->comm_sz, 1000 * (MPI_Wtime() - total_time));
	/* merge_print_array(data); */
	merge_finallize();
	ret = sorted(data->array, data->size); // 1 if array is sorted
	free(data->array);
	free(data);
	return !ret;
}

merge_mpi_data *merge_init() {
	MPI_Init(NULL, NULL);
	merge_mpi_data *ret = malloc(sizeof(merge_mpi_data));
	assert__(ret, "Failed malloc at merge_init.");
	MPI_Comm_size(MPI_COMM_WORLD, &ret->comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &ret->my_rank);
	assert__(!(ret->comm_sz % 2), "Expected a even number of MPI process.");
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
	assert__(data->array, "Failed malloc at merge_generate_random_array.");
	data->size = size;
	for (int i = 0; i < size; ++i) {
		data->array[i] = rand();
	}
	return;
}

void MPI_merge_sort(merge_mpi_data *data) {
	int step = data->size / data->comm_sz;
	if (data->my_rank == 0) { // Process 0 splits the array and sends to all other process
		int rem  = data->size % data->comm_sz;
		for (int i = 1; i < data->comm_sz; ++i) { // Skips 0 so it doesn't send a message to itself
			data->copy_time = MPI_Wtime();
			MPI_Send(&step                       , 1   , MPI_INT, i, 0, MPI_COMM_WORLD); // Sends array's size
			MPI_Send(&data->array[(step*i) + rem], step, MPI_INT, i, 1, MPI_COMM_WORLD); // Sends array at propper start
			data->copy_time = MPI_Wtime() - data->copy_time;
		}
		data->size = step + (data->size % data->comm_sz); // Gets one part of the array + the remain
	} else { // Other process just recives it
		data->copy_time = MPI_Wtime();
		MPI_Recv(&data->size, 1         , MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(data->array, data->size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		data->copy_time = MPI_Wtime() - data->copy_time;
	}
	data->process_time = MPI_Wtime();
	int *sorted_array = merge_sort(data->array, 0, data->size);
	data->process_time = MPI_Wtime() - data->process_time;
	free(data->array);
	data->array = sorted_array;
	long double aux_time;
	if (data->my_rank == 0) { // Process 0 gets the sorted arrays and merger to its own
		int *aux = malloc(sizeof(int) * data->size);
		for (int i = 1; i < data->comm_sz; ++i) { // Skips 0 so it doesn't send a message to itself
			aux_time = MPI_Wtime();
			MPI_Recv(aux, step, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			data->copy_time += MPI_Wtime() - aux_time;
			aux_time = MPI_Wtime();
			sorted_array = merge_stack(data->array, data->size, aux, step);
			data->process_time += MPI_Wtime() - aux_time; 
			free(data->array);
			data->array = sorted_array;
			data->size += step;
		}
	} else { // Other process just sends away the sorted array back to process 0
		aux_time = MPI_Wtime();
		MPI_Send(data->array, data->size, MPI_INT, 0, 0, MPI_COMM_WORLD);
		data->copy_time += MPI_Wtime() - aux_time;
	}
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
	assert__(new_array, "Falled malloc at merge_stack.");
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

void merge_tell_times(merge_mpi_data *data) {
	printf("%i, %i, %Lf, %Lf\n", data->comm_sz, data->my_rank, 1000 * data->process_time, 1000 * data->copy_time);
}

void merge_print_array(merge_mpi_data *data) {
	if (data->my_rank != 0)
		return;
	printf("Full array:\n");
	printf("[");
	int i;
	for (i = 0; i < data->size - 1; ++i) {
		printf("%i, ", data->array[i]);
	}
	printf( "%i]\n", data->array[data->size - 1]);
	return;
}

int sorted(int *array, int size) {
	int result = 1;
	for (int i = 0; i < size-1 && result; ++i) {
		result = array[i] <= array[i+1];
	}
	return result;
}
