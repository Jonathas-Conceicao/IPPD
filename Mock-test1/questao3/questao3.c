# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <mpi.h>

int main (int argc, char *argv[]);
int prime_number (int n, int id, int p);
void timestamp ();

int main (int argc, char *argv[]) {
	int id;
	int n;
	int n_hi;
	int p;

	if (argc >= 2) {
		n_hi = atoi(argv[1]);
	} else {
		n_hi = 262144;
	}
	MPI_Init ( &argc, &argv );
	MPI_Comm_size ( MPI_COMM_WORLD, &p );
	MPI_Comm_rank ( MPI_COMM_WORLD, &id );

	if ( id == 0 ) {
		timestamp ( );
	}

	n = n_hi;

  prime_number ( n, id, p );
	MPI_Finalize ( );
	if ( id == 0 ) {
		timestamp ( );
	}

	return 0;
}

int prime_number (int n, int id, int p) {
	int i;
	int j;
	int prime;
	int total;

	total = 0;

	printf("[");
	for ( i = 1 + id; i <= n; i = i + p ) {
		prime = 1;
		for ( j = 2; j < i; j++ ) {
			if ( ( i % j ) == 0 ) {
				prime = 0;
				break;
			}
		}
		if (prime) {
			printf("%i, ", i);
		}
		total = total + prime;
	}
	printf("]\n");
	return total;
}

void timestamp (void) {
# define TIME_SIZE 40

	static char time_buffer[TIME_SIZE];
	const struct tm *tm;
	time_t now;

	now = time ( NULL );
	tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

	printf ( "%s\n", time_buffer );

	return;
# undef TIME_SIZE
}

