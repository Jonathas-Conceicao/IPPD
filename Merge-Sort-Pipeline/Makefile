SHELL=bash
CC=mpicc
CFLAGS=-Wall -Wextra -std=c11

EX=mpiexec

SIZE?=1000
SEED?=313
PROCESS?=4
TIMES?=10

FILE=merge_sort_pipeline_mpi
TARGET=a.out
RESULT=times.log
R_SCRIPT=generateTables.R
GRAPH=Medidas

all: $(TARGET)

test: $(TARGET)
	$(EX) -n $(PROCESS) ./$(TARGET) $(SIZE) $(SEED)

generate: $(TARGET) $(GRAPH).pdf

$(TARGET): $(FILE).c
	$(CC) $(FILE).c $(CFLAGS)

$(RESULT): cleanLog
	@echo "Running $(TIMES) times for avrage"
	@for((i=0;i<$(TIMES);i++)); do \
		for((j=1;j<$(PROCESS)+1;j++)); do \
			$(EX) -n $$j ./$(TARGET) $(SIZE) $(SEED) >> $(RESULT); \
		done; \
	done

$(GRAPH).pdf: $(RESULT)
	./generateTables.R $(PROCESS) $(TIMES) $(RESULT) $(GRAPH).pdf

cleanLog:
	rm -f $(RESULT)

clean: cleanLog
	rm -rf $(TARGET)
	rm -rf $(RESULT)
	killall -15 $(TARGET) || true
