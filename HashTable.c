#include "HashTable.h"

int hash(char* key) {
    int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
        hash = (hash * 31 + key[i]) % TABLE_SIZE;
    }
    return hash;
}

Node* createNode(char* key, int value) {
    Node* node = malloc(sizeof(Node));
    node->key = malloc(strlen(key) + 1);
    strcpy(node->key, key);
    node->value = value;
    node->next = NULL;
    return node;
}

HashTable* createHashTable() {
    HashTable* table = malloc(sizeof(HashTable));
    table->nodes = calloc(TABLE_SIZE, sizeof(Node*));
    return table;
}

void destroyHashTable(HashTable* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = table->nodes[i];
        while (node != NULL) {
            Node* temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->nodes);
    free(table);
}

void insert(HashTable* table, char* key, int value) {
    int index = hash(key);
    Node* node = table->nodes[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->value = value;
            return;
        }
        node = node->next;
    }
    Node* newNode = createNode(key, value);
    newNode->next = table->nodes[index];
    table->nodes[index] = newNode;
}

int get(HashTable* table, char* key) {
    int index = hash(key);
    Node* node = table->nodes[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            return node->value;
        }
        node = node->next;
    }
    return -1;
}


