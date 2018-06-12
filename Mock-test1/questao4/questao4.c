# include <stdlib.h>
# include <stdio.h>
# include <omp.h>
# include <time.h>

int main (int argc, char *argv[]);
int prime_number (int n, int id, int p);
void timestamp ();

int main (int argc, char *argv[]) {
	int n;

	printf ( "\n" );
	printf ( "  Number of processors available = %d\n", omp_get_num_procs ( ) );
	printf ( "  Number of threads =              %d\n", omp_get_max_threads ( ) );

	if (argc >= 2) {
		n = atoi(argv[1]);
	} else {
		n = 262144;
	}

	printf("[");
	int prime = 1;
	int j;
	int i;
# pragma omp parallel shared (n) private(i, j)
# pragma omp for schedule(static)
	for ( i = 1; i <= n; ++i) {
		prime = 1;
		for (j = 2; j < i; j++ ) {
			if ( ( i % j ) == 0 ) {
				prime = 0;
				break;
			}
		}
		if (prime) {
			printf("%i, ", i);
		}
	}
	printf("]\n");

	return 0;
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
