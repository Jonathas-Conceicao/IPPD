CC=gcc
CC_FLAGS=-Wall -Wextra -fopenmp

PROCESS := 2
VALUE := 256

FILE=questao4

.PHONY: all compile clean test

all: compile test

compile: $(FILE).out

test: compile
	./$(FILE).out $(VALUE)

$(FILE).out: $(FILE).c
	$(CC) $(CC_FLAGS) $(FILE).c -o $(FILE).out

clean:
	rm -f $(FILE).out
