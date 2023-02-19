#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_WORD_LEN 50
#define MAX_NUM_WORDS 100000

typedef struct word_count {
    char word[MAX_WORD_LEN];
    int count;
    struct word_count *next;
} word_count;

typedef struct hash_table {
    word_count *table[MAX_NUM_WORDS];
    int num_words;
} hash_table;

void freq_function(FILE *file, int n);
void freq_word_function(FILE *file, char *word);
void search_function(FILE *file, char **search_term, int num_search_terms);
void process_word(char *word);
void add_word(hash_table *ht, char *word);


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

    for (int i = 3; i < argc; i++) {
        char *filename = argv[i];
        FILE *file = fopen(filename, "r");

        if (!file) {
            printf("Erro: nao foi possivel abrir o arquivo: '%s'\n", filename);
            continue;
        }

        if (strcmp(function, "--freq") == 0) {
            printf("Frequencia das %d palavras mais comuns em %s:\n", freq_n, filename);
            freq_function(file, freq_n);
        } else if (strcmp(function, "--freq-word") == 0) {
            printf("Frequencia da palavra \"%s\" em %s:\n", freq_word, filename);
            freq_word_function(file, freq_word);
        } else if (strcmp(function, "--search") == 0) {
            printf("O termo \"%s\" aparece nas seguintes linhas de %s:\n", search_term[0], filename);
            search_function(file, search_term, num_search_terms);
        }

        fclose(file);
    }

    free(search_term);

    return 0;
}
//=====================================================FUNCOES=======================================================//

void freq_function(FILE *file, int n) {
    // inicializa a tabela hash
    hash_table ht;
    ht.num_words = 0;

    // lê cada palavra do arquivo e adiciona à tabela hash
    char word[MAX_WORD_LEN];
    while (fscanf(file, "%s", word) != EOF) {
        process_word(word); // processa a palavra antes de adicioná-la
        add_word(&ht, word);
    }

    // ordena a tabela hash em ordem decrescente de ocorrência
    for (int i = 0; i < ht.num_words - 1; i++) {
        for (int j = i + 1; j < ht.num_words; j++) {
            if (ht.table[i]->count < ht.table[j]->count) {
                word_count *temp = ht.table[i];
                ht.table[i] = ht.table[j];
                ht.table[j] = temp;
            }
        }
    }
    // imprime as n palavras mais comuns
    for (int i = 0; i < n && i < ht.num_words; i++) {
        printf("%d - %s: %d\n", i+1, ht.table[i]->word, ht.table[i]->count);
    }

    // libera a memória alocada para a tabela hash
    for (int i = 0; i < ht.num_words; i++) {
        free(ht.table[i]);
    }
}
//------------------------------------------------------------------------------------------------------------------//

void freq_word_function(FILE *file, char *word) {

    hash_table ht;
    ht.num_words = 0;

    // converte a palavra passa pelo usuário em lowercase
    int word_len = strlen(word);
    char lowercase_word[word_len + 1];
    for (int i = 0; i < word_len; i++) {
        lowercase_word[i] = tolower(word[i]);
    }
    lowercase_word[word_len] = '\0';

    // lê cada palavra do arquivo e adiciona à tabela hash
    char curr_word[MAX_WORD_LEN];
    while (fscanf(file, "%s", curr_word) != EOF) {
        process_word(curr_word); // process the word before adding it

        // convert the current word to lowercase
        int curr_word_len = strlen(curr_word);
        char lowercase_curr_word[curr_word_len + 1];
        for (int i = 0; i < curr_word_len; i++) {
            lowercase_curr_word[i] = tolower(curr_word[i]);
        }
        lowercase_curr_word[curr_word_len] = '\0';

        if (strcmp(lowercase_curr_word, lowercase_word) == 0) {
            add_word(&ht, curr_word);
        }
    }

    // print the frequency of the word
    if (ht.num_words > 0) {
        printf("A palavra \"%s\" aparece %d vezes no arquivo.\n", word, ht.table[0]->count);
    } else {
        printf("A palavra \"%s\" nao aparece no arquivo.\n", word);
    }

    // free the memory allocated for the hash table
    for (int i = 0; i < ht.num_words; i++) {
        free(ht.table[i]);
    }
}
//------------------------------------------------------------------------------------------------------------------//

void search_function(FILE *file, char **search_term, int num_search_terms) {
    // Implementação da função de busca com parâmetro term
}
//------------------------------------------------------------------------------------------------------------------//

void process_word(char *word) {

    // verifica se a palavra tem mais de 1 caractere
    if (strlen(word) < 2) {
        return;
    }

    // converte todos os caracteres para minúsculo
    for (int i = 0; word[i]; i++) {
        word[i] = tolower(word[i]);
    }

    // remove caracteres que não são letras
    int i = 0, j = 0;
    while (word[i]) {
            // função isalpha() checa se um caractere está presente no alfabeto (a - z e A - Z) ou não. Retorna "0" se o caractere não estiver no alfabeto.
        if (isalpha(word[i])) {
            word[j] = word[i];
            j++;
        }
        i++;
    }
    word[j] = '\0';
}
//------------------------------------------------------------------------------------------------------------------//

void add_word(hash_table *ht, char *word) {

    // processa a palavra
    process_word(word);

    // verifica se a palavra foi processada para uma string vazia
    if (strlen(word) <= 2) {
        return;
    }

    // procura pela palavra na tabela hash
    int index = -1;
    for (int i = 0; i < ht->num_words; i++) {
        if (strcmp(ht->table[i]->word, word) == 0) {
            index = i;
            break;
        }
    }

    // se a palavra já existe na tabela, incrementa a contagem
    if (index >= 0) {
        ht->table[index]->count++;
    }
    // caso contrário, adiciona a palavra à tabela
    else {
        word_count *wc = malloc(sizeof(word_count));
        strncpy(wc->word, word, MAX_WORD_LEN);
        wc->count = 1;
        ht->table[ht->num_words] = wc;
        ht->num_words++;
    }
}
