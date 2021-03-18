CC = gcc
WARNING_FLAGS = -Wall -Wextra -pedantic
EXE = 537pfsim-fifo 537pfsim-lru 537pfsim-clock
SCAN_BUILD_DIR = scan-build-out

all: 537pfsim-fifo.o 537pfsim-lru.o 537pfsim-clock.o CollectInfo.o Input.o PageTable.o RAM.o PagingDevice.o Statistic.o
	$(CC) -o 537pfsim-fifo 537pfsim-fifo.o CollectInfo.o Input.o PageTable.o RAM.o PagingDevice.o Statistic.o
	$(CC) -o 537pfsim-lru 537pfsim-lru.o CollectInfo.o Input.o PageTable.o RAM.o PagingDevice.o Statistic.o 
	$(CC) -o 537pfsim-clock 537pfsim-clock.o CollectInfo.o Input.o PageTable.o RAM.o PagingDevice.o Statistic.o

537pfsim-fifo.o: 537pfsim-fifo.c CollectInfo.h Input.h PageTable.h RAM.h PagingDevice.h Statistic.h 
	$(CC) $(WARNING_FLAGS) -c 537pfsim-fifo.c

537pfsim-lru.o: 537pfsim-lru.c CollectInfo.h Input.h PageTable.h RAM.h PagingDevice.h Statistic.h
	$(CC) $(WARNING_FLAGS) -c 537pfsim-lru.c

537pfsim-clock.o: 537pfsim-clock.c CollectInfo.h Input.h PageTable.h RAM.h PagingDevice.h Statistic.h
	$(CC) $(WARNING_FLAGS) -c 537pfsim-clock.c

PagingDevice.o: CollectInfo.h PageTable.h RAM.h PagingDevice.c PagingDevice.h
	$(CC) $(WARNING_FLAGS) -c PagingDevice.c

RAM.o: RAM.c RAM.h
	$(CC) $(WARNING_FLAGS) -c RAM.c

PageTable.o: CollectInfo.h PageTable.c PageTable.h
	$(CC) $(WARNING_FLAGS) -c PageTable.c 

Input.o: Input.c Input.h
	$(CC) $(WARNING_FLAGS) -c Input.c

CollectInfo.o: CollectInfo.c CollectInfo.h
	$(CC) $(WARNING_FLAGS) -c CollectInfo.c

Statistic.o: Statistic.c Statistic.h
	$(CC) $(WARNING_FLAGS) -c Statistic.c

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

