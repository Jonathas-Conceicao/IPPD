__kernel void matrix_mult_kernel(__global const int *A, __global const int *B, __global int *C, const int SIZE) {
	int rowID = get_global_id(0);
	int colID = get_global_id(1);

	int ac = 0;
	for (int i = 0; i < SIZE; ++i) {
		ac += A[i*SIZE + rowID] * B[colID*SIZE + i];
	}
	C[rowID * SIZE + colID] = ac;
}
