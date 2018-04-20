/*****************************************************/
/* Nome: Jonathas Conceição                          */
/* Github: Jonathas-Conceicao                        */
/* Search for a specific key on a cyclic linked list */
/*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

#define SIZE 1000000

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
	int keys[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		keys[i] = i;
	}
	char done = 0;
  int target = 313;
	int my_rank;
	int comm_sz;
	MPI_Request req;   // Required variable for non-blocking calls
	MPI_Status stat;

	cList_t *head = initcList(keys, SIZE);
	cList_t *curr = head;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int other_process = (my_rank + 1) % comm_sz;

	assert(comm_sz == 2);
	do {
		if (curr->key == target) { // If the key was found
			done = 1;
			printf("I, process %i, found the key, sedding signal to %i so it can stop working!\n", my_rank, other_process);
			MPI_Send(&done, 1, MPI_CHAR, other_process, 0, MPI_COMM_WORLD);
		} else {
			curr = (my_rank) ? curr->next : curr->prev; // Process search in different directions
  		MPI_Irecv(&done, 1, MPI_CHAR, other_process, 0, MPI_COMM_WORLD, &req);
  	}
		
	} while(!done);
	MPI_Wait(&req, &stat);

	MPI_Finalize();

	return 0;
}
