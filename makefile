CC = gcc
CFLAGS = -Wall -Werror -std=c99

indexer: indexer.o HashTable.o
	$(CC) $(CFLAGS) -o $@ $^

indexer.o: indexer.c HashTable.h
	$(CC) $(CFLAGS) -c $<

HashTable.o: HashTable.c HashTable.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f indexer *.o
