MPIC=mpicc
MPIC_FLAGS=-Wall -Wextra

MPIX=mpiexec
MPIX_FLAGS=-n $(PROCESS)

PROCESS := 2
VALUE := 256

FILE=questao3

.PHONY: all compile clean run

all: compile run

compile: $(FILE).out

test: compile
	$(MPIX) $(MPIX_FLAGS) ./$(FILE).out $(VALUE)

$(FILE).out: $(FILE).c
	$(MPIC) $(MPIC_FLAGS) $(FILE).c -o $(FILE).out

clean:
	rm -f $(FILE).out
