CC=mpicc
CFLAGS=-Wall -Wextra -std=c11

EX=mpiexec

SIZE?=1000
SEED?=313
PROCESS?=4
TIMES?=10

FILE = merge_sort_mpi
TARGET=a.out
RESULT=times.log
R_SCRIPT=generateTables.R

all: $(TARGET)

$(TARGET): $(FILE).c
	$(CC) $(FILE).c $(CFLAGS)

test: $(TARGET)
	$(EX) -n $(PROCESS) ./$(TARGET) $(SIZE) $(SEED)

generate: $(TARGET)
	@echo "Running $(TIMES) times for avrage"
	@for((i=0;i<$(TIMES);i++)); do \
		$(EX) -n $(PROCESS) ./$(TARGET) $(SIZE) $(SEED) >> $(RESULT) ; \
	done
	@./generateTables.R $(PROCESS) $(TIMES) $(RESULT)

clean:
	rm -rf $(TARGET)
	rm -rf $(RESULT)
	killall -15 $(TARGET) || true
