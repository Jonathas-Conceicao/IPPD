CC=gcc
C_PFLAGS=-std=c11 -Wall -Wextra
C_SFLAGS=-lOpenCL

FILE=main.c
KERNEL=vector_add_kernel.cl

OUT=a.out

.PHONY: all build run clean

all:build run

build:$(OUT)

run:$(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

$(OUT): $(FILE) $(KERNEL)
	$(CC) $(C_PFLAGS) -o $(OUT) $(FILE) $(C_SFLAGS)
