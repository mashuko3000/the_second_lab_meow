#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define YESS 1  
#define OH_NO 0
#define ERR_MEMORY_ALLOCATION -1
#define ERR_INVALID_INPUT -2

int is_Kaprecar(long long x) { 
    long long square = x * x; 
    char square_num[BUFSIZ]; 
    int len;

    sprintf(square_num, "%lld", square); 

    len = 0; 
    while (square_num[len] != '\0') { 
        len++; 
    }

    for (int i = 1; i < len; i++) { 
        char left_part[i + 1];
        strncpy(left_part, square_num, i);
        left_part[i] = '\0'; 

        char *right_part = square_num + i;

        long long left_num = strtoll(left_part, NULL, 10);
        long long right_num = strtoll(right_part, NULL, 10);

        if (left_num + right_num == x) { 
            return YESS; 
        } 
    }

    return OH_NO; 
} 

char* number_of_Kaprecar(int count, int base, ...) { 
    va_list ptr; 
    va_start(ptr, base); 

    char* result = malloc(1024 * sizeof(char)); 
    if (result == NULL) {
        va_end(ptr);
        return (char*)ERR_MEMORY_ALLOCATION;  
    }

    result[0] = '\0';

    for (int i = 0; i < count; i++) { 
        const char* num_str = va_arg(ptr, const char*);  
        long long number = strtoll(num_str, NULL, base);  

        if (number == 0) {
            va_end(ptr);
            return (char*)ERR_INVALID_INPUT;
        }

        if (is_Kaprecar(number)) { 
            strcat(result, num_str);
            strcat(result, " is a Kaprekar number\n");
        } else { 
            strcat(result, num_str);
            strcat(result, " is not a Kaprekar number\n");
        } 
    } 

    va_end(ptr); 
    return result;
} 

int main() { 
    char* result = number_of_Kaprecar(1, 10, "999");

    if (result == (char*)ERR_MEMORY_ALLOCATION) {
        printf("Ошибка: Не удалось выделить память.\n");
    } else if (result == (char*)ERR_INVALID_INPUT) {
        printf("Ошибка: Некорректный ввод числа.\n");
    } else {
        printf("%s", result);
        free(result);
    }

    return 0; 
}
