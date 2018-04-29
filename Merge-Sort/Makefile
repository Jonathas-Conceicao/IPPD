CC=mpicc
CFLAGS=-Wall -Wextra

FILE = merge_sort_mpi

EX=mpiexec

TARGET=a.out

PROCESS?=4

all: $(TARGET)

$(TARGET): $(FILE).c
	$(CC) $(FILE).c $(CFLAGS)

run: $(TARGET)
	$(EX) -n $(PROCESS) ./$(TARGET)

clean:
	rm -rf $(TARGET)
