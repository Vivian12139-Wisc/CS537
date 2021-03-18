CC = gcc
WARNING_FLAGS = -Wall -Wextra
OPTS = -pthread
EXE = prodcom
SCAN_BUILD_DIR = scan-build-out

all: main.o thread.o queue.o
	$(CC) $(OPTS) -o $(EXE) main.o thread.o queue.o

main.o: main.c queue.h thread.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c main.c

thread.o: thread.c thread.h queue.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c thread.c

queue.o: queue.c queue.h
	$(CC) $(OPTS) $(WARNING_FLAGS) -c queue.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html

