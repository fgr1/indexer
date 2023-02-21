#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "HashTable.h"

#define MAX_WORD_LENGTH 50
#define MAX_NUM_WORDS 100000

void cria_tabela_arquivo(FILE *file, HashTable *table);
void freq_n_function(int n, HashTable* table);
void freq_word_function(char *word, HashTable* table);
void search_function(FILE *file, char **search_term, int num_search_terms);
void processWord(char* word);
int compare_nodes(const void* a, const void* b);


int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Modo de uso: ./program <funcao> <complemento> <arquivo1> [<arquivo2> ...]\n");
        return 1;
    }

    char *function = argv[1];
    char *complement = argv[2];
    char **search_term = NULL;
    int num_search_terms = 0;
    int freq_n = 0;
    char *freq_word = NULL;

    if (strcmp(function, "--freq") == 0) {
        freq_n = atoi(complement);
    } else if (strcmp(function, "--freq-word") == 0) {
        freq_word = complement;
    } else if (strcmp(function, "--search") == 0) {
        int search_term_size = 1;
        search_term = malloc(sizeof(char *) * search_term_size);

        for (int i = 3; i < argc; i++) {
            if (access(argv[i], F_OK) != -1) {
                break;
            } else {
                search_term[num_search_terms] = strdup(argv[i]);
                num_search_terms++;

                if (num_search_terms >= search_term_size) {
                    search_term_size *= 2;
                    search_term = realloc(search_term, sizeof(char *) * search_term_size);
                }
            }
        }
    } else {
        printf("Erro: funcao invalida '%s'\n", function);
        return 1;
    }

    HashTable *ht = createHashTable();

    for (int i = 3; i < argc; i++) {
        char *filename = argv[i];
        FILE *file = fopen(filename, "r");

        if (!file) {
            printf("Erro: nao foi possivel abrir o arquivo: '%s'\n", filename);
            continue;
        }

        if (strcmp(function, "--freq") == 0) {
            printf("Frequencia das %d palavras mais comuns em %s:\n", freq_n, filename);
            cria_tabela_arquivo(file, ht);
            freq_n_function(freq_n, ht);
        } else if (strcmp(function, "--freq-word") == 0) {
            printf("Frequencia da palavra \"%s\" em %s:\n", freq_word, filename);
            cria_tabela_arquivo(file, ht);
            freq_word_function(freq_word, ht);
        } else if (strcmp(function, "--search") == 0) {
            printf("O termo \"%s\" aparece nas seguintes linhas de %s:\n", search_term[0], filename);
        }

        fclose(file);
    }

    free(search_term);
    free(ht);

    return 0;
}
//=====================================================FUNCOES=======================================================//

void cria_tabela_arquivo(FILE *file, HashTable *table) {
    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) == 1) { // read each word from the file
        processWord(word); // process the word
        if (strlen(word) < 2) { // check if the word is less than 2 characters
            continue; // ignore the word
        }
        int value = get(table, word); // check if the word is already in the table
        if (value == -1) {
            insert(table, word, 1); // if not, add it with a count of 1
        } else {
            insert(table, word, value + 1); // if it is, increment its count
        }
    }
}
//------------------------------------------------------------------------------------------------------------------//

void freq_n_function(int n, HashTable* table) {
    // Cria um array de ponteiros para nós da tabela hash
    Node** nodesArray = malloc(TABLE_SIZE * sizeof(Node*));
    int count = 0;

    // Percorre toda a tabela hash e adiciona todos os nós a nodesArray
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* node = table->nodes[i];
        while (node != NULL) {
            nodesArray[count] = node;
            count++;
            node = node->next;
        }
    }

    // Ordena nodesArray com base no valor de ocorrência de cada palavra
    qsort(nodesArray, count, sizeof(Node*), compare_nodes);

    // Imprime as "n" palavras com o maior valor de ocorrência em ordem decrescente
    for (int i = 0; i < n && i < count; i++) {
        printf("%d: %s - %d\n", i+1, nodesArray[i]->key, nodesArray[i]->value);
    }

    free(nodesArray);
}
//------------------------------------------------------------------------------------------------------------------//

void freq_word_function(char* word, HashTable* table) {
    int count = get(table, word);
    if (count == -1) {
        printf("A palavra '%s' nao foi encontrada na tabela.\n", word);
    } else {
        printf("A palavra '%s' apareceu %d vezes na tabela.\n", word, count);
    }
}
//------------------------------------------------------------------------------------------------------------------//

void search_function(FILE *file, char **search_term, int num_search_terms) {
    // Implementação da função de busca com parâmetro term
}
//------------------------------------------------------------------------------------------------------------------//

void processWord(char* word) {
    int i, j;
    for (i = 0, j = 0; word[i]; i++) {
        if (isalpha(word[i])) {
            word[j++] = tolower(word[i]);
        }
    }
    word[j] = '\0';
}
//------------------------------------------------------------------------------------------------------------------//

int compare_nodes(const void* a, const void* b) {
    const Node* nodeA = *(const Node**)a;
    const Node* nodeB = *(const Node**)b;

    return (nodeB->value - nodeA->value);
}


