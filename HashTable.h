#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10000

typedef struct Node {
    char* key;
    int value;
    struct Node* next;
} Node;

typedef struct {
    Node** nodes;
} HashTable;

int hash_func(char* key);
Node* createNode(char* key, int value);
HashTable* createHashTable();
void destroyHashTable(HashTable* table);
void insert(HashTable* table, char* key, int value);
int get(HashTable* table, char* key);
