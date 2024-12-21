#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define PTR_ERROR -1
#define OK 0;

int tokenize(char* initial, int (*detector)(int), int accept_empty_lexems, char*** lexems, size_t* lexems_count);
int is_separaror(int c);
char *** temp = NULL;

int main()
{
    char input[] = "Lublu Lyublino i cruteuh nakachenneuh muzhikov   ";
    char** lexems =NULL;
    size_t lexems_count = 0;

    int result = tokenize(input, is_separaror, 0, &lexems, &lexems_count);

    if (result == 0) {
        for (size_t i = 0; i < lexems_count; i++) {
            printf("Lexem %zu: '%s'\n", i, lexems[i]);
            free(lexems[i]);
        }
        free(lexems); 
    } else {
        printf("Error during tokenization.\n");
    }

    return 0;
}

int is_separaror(int c)
{
    return (c == ' ' || c == '\t');
}

int tokenize(char* initial, int (*detector)(int), int accept_empty_lexems, char*** lexems, size_t* lexems_count) {
    if (!initial || !detector || !lexems || !lexems_count) {
        return PTR_ERROR; 
    }

    size_t capacity = 10; 
    *lexems = (char**)malloc(capacity * sizeof(char*));
    if (*lexems == NULL) {
        return PTR_ERROR; 
    }

    *lexems_count = 0;
    size_t len = strlen(initial);
    char* current_lexem = NULL;
    size_t current_lexem_len = 0;

    for (size_t i = 0; i <= len; i++) {
        char c = initial[i]; 

        if (detector(c) || c == '\0') {
            if (current_lexem != NULL) {
                if (accept_empty_lexems || current_lexem_len > 0) {
                    if (*lexems_count >= capacity) {
                        capacity *= 2;
                        *temp = (char**)realloc(*lexems, capacity * sizeof(char*));
                        if (*temp == NULL) {
                            free(*lexems);
                            return PTR_ERROR; 
                        }
                        *lexems = *temp;
                    }
                    (*lexems)[*lexems_count] = current_lexem;
                    (*lexems_count)++;
                }
                current_lexem = NULL;
            }
        } else { 
            if (current_lexem == NULL) {
                current_lexem = (char*)malloc(sizeof(char) * (len + 1));
                if (current_lexem == NULL) {
                    return PTR_ERROR; 
                }
                current_lexem_len = 0;
            }
            current_lexem[current_lexem_len++] = c;
            current_lexem[current_lexem_len] = '\0'; 
        }
    }

    return OK;
}