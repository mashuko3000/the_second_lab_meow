#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ERR_MEMORY_ALLOCATION -1
#define ERR_INVALID_INPUT -2
#define ERR_NULL_POINTER -3

int char_to_digit(char c, int base) {
    if (isdigit(c)) {
        return c - '0';
    } else if (isalpha(c)) {
        return tolower(c) - 'a' + 10;
    }
    return ERR_INVALID_INPUT;
}

unsigned long long str_to_decimal(const char *num, int base) {
    if (num == NULL) {
        return ERR_NULL_POINTER;
    }

    unsigned long long result = 0;
    while (*num) {
        int digit = char_to_digit(*num, base);
        if (digit == ERR_INVALID_INPUT) {
            return ERR_INVALID_INPUT;
        }
        result = result * base + digit;
        num++;
    }
    return result;
}

int decimal_to_str(unsigned long long num, int base, char **result) {
    if (result == NULL) {
        return ERR_NULL_POINTER;
    }

    const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *temp = (char *)malloc(64 * sizeof(char));
    if (temp == NULL) {
        free(temp);
        return ERR_MEMORY_ALLOCATION;
    }

    int index = 0;

    if (num == 0) {
        temp[index++] = '0';
    } else {
        while (num > 0) {
            temp[index++] = digits[num % base];
            num /= base;
        }
        for (int i = 0; i < index; i++) {
            temp[i] = temp[index - i - 1];
        }
    }
    temp[index] = '\0';

    *result = temp;

    return 0;
}

int add_numbers(int base, const char *num1, const char *num2, char **result) {
    if (num1 == NULL || num2 == NULL || result == NULL) {
        return ERR_NULL_POINTER;
    }

    unsigned long long n1 = str_to_decimal(num1, base);
    if (n1 < 0) return n1;

    unsigned long long n2 = str_to_decimal(num2, base);
    if (n2 < 0) return n2;

    unsigned long long sum = n1 + n2;
    return decimal_to_str(sum, base, result);
}

int sum_numbers(int base, int count, char **result, ...) {
    if (result == NULL) {
        return ERR_NULL_POINTER;
    }

    va_list args;
    va_start(args, result);

    char *current_num = va_arg(args, char*);
    if (current_num == NULL) {
        va_end(args);
        return ERR_NULL_POINTER;
    }

    *result = (char *)malloc(strlen(current_num) + 1);
    if (*result == NULL) {
        free(*result);
        va_end(args);
        return ERR_MEMORY_ALLOCATION;
    }
    strcpy(*result, current_num);

    for (int i = 1; i < count; i++) {
        current_num = va_arg(args, char*);
        if (current_num == NULL) {
            va_end(args);
            return ERR_NULL_POINTER;
        }

        char *temp_result = NULL;
        int err = add_numbers(base, *result, current_num, &temp_result);
        if (err != 0) {
            va_end(args);
            return err;
        }

        free(*result);
        *result = temp_result;
    }

    va_end(args);
    return 0;
}

void remove_leading_zeros(char *str) {
    if (str == NULL) {
        return;
    }

    while (*str == '0' && *(str + 1) != '\0') {
        str++;
    }
}

void handle_error(int error_code) {
    switch (error_code) {
        case ERR_MEMORY_ALLOCATION:
            printf("Ошибка: Не удалось выделить память.\n");
            break;
        case ERR_INVALID_INPUT:
            printf("Ошибка: Некорректный ввод числа.\n");
            break;
        case ERR_NULL_POINTER:
            printf("Ошибка: NULL указатель.\n");
            break;
        default:
            printf("Неизвестная ошибка.\n");
            break;
    }
}

int main() {
    char *result = NULL;

    int err = sum_numbers(32, 3, &result, "pdd", "ad", "3b");
    if (err != 0) {
        handle_error(err);
    } else {
        printf("Сумма чисел в 32-й системе: %s\n", result);
        free(result);
    }

    err = sum_numbers(10, 4, &result, "123", "456", "789", "0000101");
    if (err != 0) {
        handle_error(err);
    } else {
        printf("Сумма чисел в 10-й системе: %s\n", result);
        free(result);
    }

    return 0;
}
