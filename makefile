OBJS	= indexer.o HashTable.o
SOURCE	= indexer.c HashTable.c
HEADER	= HashTable.h
OUT	= indexer
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) 

main.o: main.c
	$(CC) $(FLAGS) indexer.c 

theTrie.o: theTrie.c
	$(CC) $(FLAGS) HashTable.c

clean:
	rm -f $(OBJS) $(OUT)
