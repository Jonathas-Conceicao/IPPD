CC=mpicc
CFLAGS=-Wall -Wextra -std=c11
SIZE?=1000
SEED?=313

FILE = merge_sort_mpi

EX=mpiexec

TARGET=a.out

PROCESS?=4

all: $(TARGET)

$(TARGET): $(FILE).c
	$(CC) $(FILE).c $(CFLAGS)

run: $(TARGET)
	$(EX) -n $(PROCESS) ./$(TARGET) $(SIZE) $(SEED)

clean:
	rm -rf $(TARGET)
