CC  = gcc
CFLAGS  = -Wall -pedantic
OBJECTS = 537ps.o opProc.o getProcList.o parseInfo.o printOps.o

537ps: $(OBJECTS)
	$(CC) $(CFLAGS) -o 537ps $(OBJECTS)

537ps.o: 537ps.c opProc.h getProcList.h parseInfo.h printOps.h
	$(CC) $(CFLAGS) -c 537ps.c

opProc.o: opProc.c opProc.h
	$(CC) $(CFLAGS) -c opProc.c

getProcList.o: getProcList.c getProcList.h
	$(CC) $(CFLAGS) -c getProcList.c

parseInfo.o: parseInfo.c parseInfo.h
	$(CC) $(CFLAGS) -c parseInfo.c

printOps.o: printOps.c printOps.h opProc.h parseInfo.h
	$(CC) $(CFLAGS) -c printOps.c

clean:
	rm -f $(OBJECTS) 537ps
