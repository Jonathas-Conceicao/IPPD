/*****************************************************/
/* Nome: Jonathas Conceição                          */
/* Github: Jonathas-Conceicao                        */
/* Search for a specific key on a cyclic linked list */
/*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

typedef struct cList_ {
	int key;
	struct cList_ *next;
	struct cList_ *prev;
} cList_t;

cList_t *newNodecList(int key, cList_t *next, cList_t *prev) {
	cList_t *node;
	node = malloc(sizeof(cList_t));
	assert(node);
	node->key = key;

	if (next) node->next = next;
	else node->next = node;
	if (prev) node->prev = prev;
	else node->prev = node;

	return node;
}

void insertNewKeycList(cList_t **head, int key) {
	cList_t *node;
	node = newNodecList(key, (*head), (*head)->prev);
	(*head)->prev->next = node;
	(*head)->prev = node;
	*head = node;
}


cList_t *initcList(int array[], int size) {
	assert(size >= 1);
	cList_t *head;
	head = newNodecList(array[0], NULL, NULL);
	for (int i = 1; i < size; ++i) {
		insertNewKeycList(&head, array[i]);
	}
	return head;
}

void printAllcList(cList_t *head) {
	cList_t *curr = head;
	do {
		printf("Key: %i\n", curr->key);
		curr = curr->prev;
	} while (curr != head);
}

int main (void) {
	int keys[1000];
	for (int i = 0; i < 1000; ++i) {
		keys[i] = i;
	}
	char done = 0;
	int target = 313;
	int my_rank;
	int comm_sz;
	MPI_Request req;   // Required variable for non-blocking calls

	cList_t *head = initcList(keys, 1000);
	cList_t *curr = head;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int other_process = (my_rank + 1) % comm_sz;

	assert(comm_sz == 2);
	do {
		if (curr->key == target) { // If the key was found
			done = 1;
			printf("I, process %i, found the key!\n", my_rank);
			MPI_Send(&done, 1, MPI_CHAR, other_process, 0, MPI_COMM_WORLD);
		} else {			
			/* printf("Process %i fund %i, will keep looking!\n", my_rank, curr->key); */
			curr = (my_rank) ? curr->next : curr->prev; // Process search in different directions
			MPI_Irecv(&done, 1, MPI_CHAR, other_process, 0, MPI_COMM_WORLD, &req);
		}
	} while(!done);
	
	/* do { // I if were you, I would not try to understand this. */
	/* 	MPI_Recv(&token, 1, MPI_CHAR, prev_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); */
	/* 	if (token > 0) { */
	/* 		token -= 1; */
	/* 		printf("I, thread %i, am passing the token number %i.\n", my_rank, token); */
	/* 		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD); */
	/* 	} else if (token == 0) { */
	/* 		token = -my_rank -1; */
	/* 		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD); */
	/* 		token = 0; */
	/* 	} else if (token == (-my_rank - 1)) { */
	/* 		token = 0; */
	/* 	} else { */
	/* 		MPI_Send(&token, 1, MPI_CHAR, next_process, 0, MPI_COMM_WORLD); */
	/* 		token = 0; */
	/* 	} */
	/* } while (token); */
	MPI_Finalize();

	return 0;
}
