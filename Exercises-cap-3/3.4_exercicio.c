#include <stdio.h>
#include <mpi.h> 

int main(void) {
	 int my_rank, comm_sz;

	 MPI_Init(NULL, NULL); 
	 MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
	 MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

	 if (my_rank) {
		 MPI_Send(&my_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
	 } else {
		 int id;
		 printf("Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);
		 for (int i = 1; i < comm_sz; ++i) {
			 MPI_Recv(&id, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			 printf("Proc %d of %d > Does anyone have a toothpick?\n", id, comm_sz);
		 }
	 }
	 MPI_Finalize();
	 return 0;
}	 /* main */
