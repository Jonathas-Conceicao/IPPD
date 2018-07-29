#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#define MAX_SOURCE_SIZE (0x100000)

/* #define MATRIX_SIZE 10000 */
#define MATRIX_SIZE 16 // Smaller to quick tests
#define WORK_SIZE 64
#define RANDOM_SEED 313
#define MAX_RAND 100

int* new_random_matrix(int size, int empty);
void matrix_mult(int *A, int *B, int *C);
void matrix_display(int *M);

int main(void) {
	int *A;
	int *B;
	int *C;
	srand(RANDOM_SEED);

	A = new_random_matrix(MATRIX_SIZE, 0);
	B = new_random_matrix(MATRIX_SIZE, 0);
	C = new_random_matrix(MATRIX_SIZE, 1);
	printf("Matrix generated\n");

	/* matrix_display(A); */
	/* matrix_display(B); */
	matrix_mult(A, B, C);
	matrix_display(C);

	free(A);
	free(B);
	free(C);
	return 0;
}

int* new_random_matrix(int size, int empty) {
	int *matrix;
  size *= size;
	matrix = malloc(sizeof(int) * size);
	if (!matrix) {
		fprintf(stderr, "Failed to allocated memory for Matrix");
		exit(1);
	}
	if (empty) return matrix;
#pragma omp parallel for schedule(static)
	for(int i = 0; i < size; ++i) {
		matrix[i] = rand() % MAX_RAND; // Rand function is thread safe
	}
	return matrix;
}

int* new_empty_matrix(int size) {
	int *matrix;
  size *= size;
	matrix = malloc(sizeof(int) * size);
	if (!matrix) {
		fprintf(stderr, "Failed to allocated memory for Matrix");
		exit(1);
	}
	return matrix;
}

void matrix_display(int *M) {
	int i, j;
	printf("[");
	for (i = 0; i < MATRIX_SIZE; ++i) {
		printf("[");
		for (j = 0; j < MATRIX_SIZE; ++j) {
			printf("%i, ", M[(i * MATRIX_SIZE) + j]);
		}
		printf("\b\b]\n");
	}
	printf("]\n");
}

void matrix_mult(int *A, int *B, int *C) {
	// Load the kernel source code into the array source_str
	char *source_str;
	size_t source_size;
	FILE *fp;
	fp = fopen("matrix_mult_kernel.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(2);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread( source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose( fp );

	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;	 
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, 
												&device_id, &ret_num_devices);

	cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, 
																		MATRIX_SIZE * MATRIX_SIZE * sizeof(int), NULL, &ret);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
																		MATRIX_SIZE * MATRIX_SIZE * sizeof(int), NULL, &ret);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
																	  MATRIX_SIZE * MATRIX_SIZE * sizeof(int), NULL, &ret);


	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
														 MATRIX_SIZE * MATRIX_SIZE * sizeof(int), A, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0, 
														 MATRIX_SIZE * MATRIX_SIZE * sizeof(int), B, 0, NULL, NULL);

	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1, 
																								 (const char **)&source_str, (const size_t *)&source_size, &ret);

	// Build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "matrix_mult_kernel", &ret);

	// Set the arguments of the kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);
		
	// Execute the OpenCL kernel on the list
	size_t global_item_size = MATRIX_SIZE * MATRIX_SIZE; // Process the entire lists
	size_t local_item_size = WORK_SIZE; // Divide work items into groups of 64
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, 
															 &global_item_size, &local_item_size, 0, NULL, NULL);

	// Read the memory buffer C on the device to the local variable C
	ret = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0, 
														MATRIX_SIZE * MATRIX_SIZE * sizeof(int), C, 0, NULL, NULL);

	// Clean up
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(a_mem_obj);
	ret = clReleaseMemObject(b_mem_obj);
	ret = clReleaseMemObject(c_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
}
