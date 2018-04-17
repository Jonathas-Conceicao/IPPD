/****************************************************/
/* Nome: Jonathas Conceição                         */
/* Github: Jonathas-Conceicao                       */
/* Make a token cycle through a ring of MPI process */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (void) {
	int comm_sz;
	int my_rank;
	char token;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int prev_process = (my_rank + comm_sz - 1) % comm_sz;
	int next_process = (my_rank + 1) % comm_sz;
	if (my_rank == 0) { // Process 0 gets the first token.
		token = my_rank;
		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);
		MPI_Recv(&token, 1, MPI_CHAR, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %i gor a token from %i\n",my_rank, token);
	} else {
		MPI_Recv(&token, 1, MPI_CHAR, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %i gor a token from %i\n",my_rank, token);
		token = my_rank;
		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);		
	}
	MPI_Finalize();
	return 0;
}
