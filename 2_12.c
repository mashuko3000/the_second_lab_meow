#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define NOT_STRING 1
#define NOT_RESULT 2
#define NULL_POSITION 3
#define NULL_COUNT 4
#define MEMORY_ERROR 5

int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...);

void print_results(char** results, int** positions, size_t results_count) {
    for (size_t i = 0; i < results_count; i++) {
        printf("String: %s\n", results[i]);
        printf("Positions: ");
        int j = 0;
        while (positions[i][j] != -1) { //-1 окончание списка позиций
            printf("%d ", positions[i][j]);
            j++;
        }
        printf("\n");
    }
}

int main()
{
    char** results;
    int** positions;
    size_t results_count;

    int status = substr("test", 0, &results, &positions, &results_count, "This is a test string.", "Another test case.", "No match here.", NULL);
    if (status == 0) {
        printf("Substring found in %zu strings.\n", results_count);
        print_results(results, positions, results_count);

        for (size_t i = 0; i < results_count; i++) {
            free(positions[i]);
        }
        free(results);
        free(positions);
    } else {
        printf("Error: %d\n", status);
    }
    return 0;
}

int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...)
{
    if (results == NULL) return NOT_RESULT;
    if (positions == NULL) return NULL_POSITION;
    if (results_count == NULL) return NULL_COUNT;
    if (to_find == NULL) return NOT_STRING;

    char* to_find_lower = strdup(to_find);
    if (!to_find_lower) return MEMORY_ERROR;
    
    if (case_sensitive == 0) {
        for (size_t i = 0; i < strlen(to_find); i++) {
            to_find_lower[i] = tolower(to_find[i]);
        }
    }

    va_list args;
    va_start(args, results_count);

    int* buffer = NULL;
    int p = 0;
    int size_p = 10;

    *positions = (int**)malloc(sizeof(int*) * size_p);
    *results = (char**)malloc(sizeof(char*) * size_p);
    if (*positions == NULL || *results == NULL) {
        free(*positions);
        free(*results);
        free(to_find_lower);
        return MEMORY_ERROR;
    }

    *results_count = 0;

    char* current_str;
    while ((current_str = va_arg(args, char*)) != NULL) {
        int k = 0;
        int size = 10;
        buffer = (int*)malloc(sizeof(int) * size);
        if (buffer == NULL) {
            va_end(args);
            free(to_find_lower);
            return MEMORY_ERROR;
        }

        char* current_str_lower = strdup(current_str);
        if (case_sensitive == 0) {
            for (size_t i = 0; i < strlen(current_str); i++) {
                current_str_lower[i] = tolower(current_str[i]);
            }
        }

        size_t len_substr = strlen(to_find_lower);
        size_t len_str = strlen(current_str_lower);

        for (size_t i = 0; i <= len_str - len_substr; i++) {
            if (strncmp(&current_str_lower[i], to_find_lower, len_substr) == 0) {
                if (k >= size) {
                    size *= 2;
                    int* temp = (int*)realloc(buffer, sizeof(int) * size);
                    if (temp == NULL) {
                        free(buffer);
                        free(current_str_lower);
                        va_end(args);
                        free(to_find_lower);
                        return MEMORY_ERROR;
                    }
                    buffer = temp;
                }
                buffer[k++] = i;
            }
        }

        if (k > 0) {
            buffer[k] = -1;
            (*positions)[*results_count] = buffer;
            (*results)[*results_count] = current_str;
            (*results_count)++;
        } else {
            free(buffer);
        }

        free(current_str_lower);

        if (*results_count >= size_p - 1) {
            size_p *= 2;
            *positions = (int**)realloc(*positions, sizeof(int*) * size_p);
            *results = (char**)realloc(*results, sizeof(char*) * size_p);
            if (*positions == NULL || *results == NULL) {
                free(to_find_lower);
                va_end(args);
                return MEMORY_ERROR;
            }
        }
    }

    va_end(args);
    free(to_find_lower);
    return OK;
}
