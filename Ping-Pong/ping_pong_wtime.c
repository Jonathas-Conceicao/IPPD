/***********************************************/
/* Nome: Jonathas Conceição                    */
/* Github: Jonathas-Conceicao                  */
/* Make a token bounce between two MPI process */
/***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <assert.h>

#define BOUNCES 10

double avg (double v_list[], int size) {
	double ac = 0;
	for (int i = 0; i < size; i++) {
		ac += v_list[i];
	}
	return (ac / size);
}

int main (void) {
	int comm_sz;
	int my_rank;
	int *ball;
	ball = calloc(0, sizeof(int));
	double s_time;
	double e_time[BOUNCES];

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	assert(comm_sz == 2);
	assert(BOUNCES);
	for (int i = 0; i < BOUNCES; i++) {
		s_time = MPI_Wtime();
		if (my_rank == *ball) {
			*ball = (my_rank +1) % 2;
			printf("It is I, thread %i, who hold the ball at turn %i!\n", my_rank, i);
			MPI_Send(ball, 1, MPI_INT, *ball, 0, MPI_COMM_WORLD);
			e_time[i] = MPI_Wtime() - s_time;
		} else {
			MPI_Recv(ball, 1, MPI_INT, (my_rank +1) % 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	MPI_Finalize();
	if (my_rank == 0) printf("The avarege time for the bounce was: %e\n", avg(e_time, BOUNCES)); // Only the main process prints the message

	free(ball);
	return 0;
}
