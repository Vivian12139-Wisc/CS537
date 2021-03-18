CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537make
SCAN_BUILD_DIR = scan-build-out

all: 537make.o TextParser.o SpecGraph.o SpecRepresent.o Run.o Stack.o
	$(CC) -o $(EXE) 537make.o TextParser.o SpecGraph.o SpecRepresent.o Run.o Stack.o

537make.o: 537make.c TextParser.h SpecGraph.h SpecRepresent.h
	$(CC) $(WARNING_FLAGS) -c 537make.c

TextParser.o: TextParser.c TextParser.h
	$(CC) $(WARNING_FLAGS) -c TextParser.c

SpecGraph.o: TextParser.h SpecGraph.c SpecGraph.h
	$(CC) $(WARNING_FLAGS) -c SpecGraph.c

SpecRepresent.o: TextParser.h Stack.h Run.h SpecRepresent.c SpecRepresent.h
	$(CC) $(WARNING_FLAGS) -c SpecRepresent.c 

Run.o: Run.c Run.h
	$(CC) $(WARNING_FLAGS) -c Run.c

Stack.o: Stack.c Stack.h
	$(CC) $(WARNING_FLAGS) -c Stack.c

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

