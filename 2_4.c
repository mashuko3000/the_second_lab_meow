#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define OK 0
#define ERR_FILE_NOT_FOUND 1
#define ERR_PTR_IS_NOT_PTRING 2
#define ERR_FILE_READ 3

int find_substring(const char *str, const char *substr) {
    if (str == NULL || substr == NULL) {
        return ERR_PTR_IS_NOT_PTRING;
    }

    int i = 0;
    while (str[i] != '\0') {
        int j = 0;
        while (str[i + j] == substr[j] && substr[j] != '\0') {
            j++;
        }

        if (substr[j] == '\0') {
            return i;  
        }
        i++; 
    }

    return -1;
}

int search_in_file(const char *filename, const char *substring, int **results, int *count, int *capacity) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return ERR_FILE_NOT_FOUND;
    }

    int *temp = NULL;

    char str[BUFSIZ];
    int counter_str = 1;
    while (fgets(str, sizeof(str), file)) {
        int index = 0;
        while ((index = find_substring(str + index, substring)) != -1) {

            if (*count >= *capacity) {
                *capacity *= 2;

                temp = (int*)realloc(*results, *capacity * sizeof(int));
                if (temp == NULL) {
                    free(temp);
                    fclose(file);
                    return ERR_PTR_IS_NOT_PTRING; 
                }
                *results = temp; 
            }

            (*results)[*count] = counter_str;
            (*results)[*count + 1] = index + 1;
            (*count) += 2;
            index++; 
        }
        counter_str++;
    }

    fclose(file);
    return OK;
}

int search_in_files(const char *substring, int num_files, int **results, int *count, int *capacity, ...) {
    va_list files;
    va_start(files, capacity);

    for (int i = 0; i < num_files; i++) {
        const char *filename = va_arg(files, const char*);
        int result = search_in_file(filename, substring, results, count, capacity);
        if (result != OK) {
            va_end(files);
            return result;
        }
    }

    va_end(files);
    return OK;
}

int main() {
    int *results = (int*)malloc(1000 * sizeof(int)); 
    if (results == NULL) {
        printf("Ошибка при выделении памяти\n");
        return ERR_PTR_IS_NOT_PTRING;
    }
    
    int count = 0; 
    int capacity = 1000; 

    int result = search_in_files("035veakideash", 3, &results, &count, &capacity, "file1.txt", "file2.txt", "file3.txt");

    if (result == OK) {
        for (int i = 0; i < count; i += 2) {
            printf("File: %s, String: %d, Position: %d\n", 
                (i / 2 == 0) ? "file1.txt" : ((i / 2 == 1) ? "file2.txt" : "file3.txt"),
                results[i], results[i+1]);
        }
    } else {
        switch (result) {
            case ERR_FILE_NOT_FOUND:
                printf("Ошибка: файл не найден\n");
                break;
            case ERR_PTR_IS_NOT_PTRING:
                printf("Ошибка: передан NULL указатель\n");
                break;
            case ERR_FILE_READ:
                printf("Ошибка при чтении файла\n");
                break;
            default:
                printf("Неизвестная ошибка\n");
        }
    }

    free(results); 
    return 0;
}
