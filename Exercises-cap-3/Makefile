SHELL=bash
CC=mpicc
CFLAGS=-Wall -Wextra -std=c11

EX=mpiexec

FILE?=test.c
PROCESS?=4
ARGS?=
TARGET=a.out

all: $(TARGET)

test: $(TARGET)
	$(EX) -n $(PROCESS) ./$(TARGET) $(ARGS)

$(TARGET): $(FILE)
	$(CC) $(FILE) $(CFLAGS)

clean:
	rm -f $(TARGET)
	killall -15 $(TARGET) || true
