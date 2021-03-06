/****************************************************************/
/* Nome: Jonathas Conceição                                     */
/* Github: Jonathas-Conceicao                                   */
/* Make a token be passed N times through a ring of MPI process */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

#define PASSES 100

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
		token = PASSES - 1;
		printf("I, thread %i, am passing the token number %i.\n", my_rank, token);
		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);
	}
	do { // I if were you, I would not try to understand this.
		MPI_Recv(&token, 1, MPI_CHAR, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (token > 0) {
			token -= 1;
			printf("I, thread %i, am passing the token number %i.\n", my_rank, token);
			MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);
		} else if (token == 0) {
			token = -my_rank -1;
			MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);
			token = 0;
		} else if (token == (-my_rank - 1)) {
			token = 0;
		} else {
			MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD);
			token = 0;
		}
	} while (token);
	MPI_Finalize();

	return 0;
}
