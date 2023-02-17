#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 50
#define MAX_NUM_WORDS 100000

typedef struct word_count {
    char word[MAX_WORD_LEN];
    int count;
} word_count;

typedef struct hash_table {
    word_count *table[MAX_NUM_WORDS];
    int num_words;
} hash_table;

void freq_function(FILE *file, int n);
void freq_word_function(FILE *file, char *word);
void search_function(FILE *file, char *term);
void process_word(char *word);
void add_word(hash_table *ht, char *word);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Modo de uso: ./program <funcao> <complemento> <arquivo1> [<arquivo2> ...]\n");
        return 1;
    }
//------------------------------------------Recebe argumentos da main-----------------------------------------------//

    char *function = argv[1];
    char *complement = argv[2];

    int freq_n;
    char *freq_word;
    char *search_term;

    if (strcmp(function, "--freq") == 0) {
        freq_n = atoi(complement);
    } else if (strcmp(function, "--freq-word") == 0) {
        freq_word = complement;
    } else if (strcmp(function, "--search") == 0) {
        search_term = complement;
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
//------------------------------------------------------------------------------------------------------------------//

        if (strcmp(function, "--freq") == 0) {
            printf("Frequencia das %d palavras mais comuns em %s:\n", freq_n, argv[i]);
            freq_function(file, freq_n);
        } else if (strcmp(function, "--freq-word") == 0) {
            printf("Frequencia da palavra \"%s\" em %s:\n", freq_word, argv[i]);
            freq_word_function(file, freq_word);
        } else if (strcmp(function, "--search") == 0) {
            printf("O termo \"%s\" aparece nas seguintes linhas de %s:\n", search_term, argv[i]);
            search_function(file, search_term);
        }

        fclose(file);
    }

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
}

void freq_word_function(FILE *file, char *word) {
    // Implementação da função de frequência de palavra com parâmetro word
}
//------------------------------------------------------------------------------------------------------------------//

void search_function(FILE *file, char *term) {
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
