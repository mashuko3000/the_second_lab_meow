#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NOT_A_VALUE 0
#define OK 1
#define MEMORY_ERROR 2

// битовое сложения мяу
unsigned int summation(unsigned int a, unsigned int b) {
    while (b != 0) {
        unsigned int carry = a & b;  // перенос по битикам
        a = a ^ b;                   // сложение без учёта переноса
        b = carry << 1;            
    }
    return a;
}

// минусик мяу
unsigned int subtraction(unsigned int a, unsigned int b) {
    return summation(a, summation(~b, 1));  // a + (~b + 1)
}

// меняем чиселку в сс с основанием 2^r
void to_base_r(unsigned int num, unsigned int r, unsigned char *result, unsigned int *length) {
    if (result == NULL){
        printf("Ошибка: не удалось выделить память для работы программы..\n");
        return MEMORY_ERROR;
    }
    if (length == NULL){
        printf("Ошибка: не удалось выделить память для работы программы..\n");
        return MEMORY_ERROR;
    }
    unsigned int base = 1 << r;
    unsigned int mask = base - 1;
    unsigned int index = 0;

    if (num == 0) {
        result[0] = 0;
        *length = 1;
        return;
    }

    while (num > 0) {
        unsigned int digit = num & mask;
        result[index] = digit;
        index = summation(index, 1);  // тут раньше был index++
        num >>= r;
    }

    *length = index;
}

int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return NOT_A_VALUE;
        }
        str++;
    }
    return OK;
}

int main() {
    unsigned int num;
    unsigned int r;
    char input[100];
    unsigned char *result;  
    unsigned int length;    
    size_t result_size; 

    while (1) {
        printf("Введите число, целое неотрицательное, которое будет переводиться в сс с основанием 2 в степени r: ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        if (is_number(input)) {
            num = atoi(input);
            break;
        } else {
            printf("Ошибка: Ввод должен быть числом.\n");
        }
    }

    while (1) {
        printf("Введите значение r (1-5), степень числа, в которую будет возводиться 2: ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        if (is_number(input)) {
            r = atoi(input);
            if (r >= 1 && r <= 5) {
                break;
            } else {
                printf("Ошибка: Значение r должно быть в пределах от 1 до 5.\n");
            }
        } else {
            printf("Ошибка: Ввод должен быть числом.\n");
        }
    }

    // тут считаю максимульную длину для числа, ориентируюсь на сс с основанием 2, мяу
    result_size = sizeof(unsigned char) * ((sizeof(unsigned int) * 8) / r + 1); // немножечко страхуемся прибавляя единицу xd

    // тут у меня выделяется память с ПРОВЕРКОЙ
    result = (unsigned char *)malloc(result_size);
    if (result == NULL) {
        printf("Ошибка: недостаточно памяти для хранения результата.\n");
        return MEMORY_ERROR;
    }

    to_base_r(num, r, result, &length);

    // А Я ТЕПЕРЬ ТУТ ПЕЧАТАЮ РЕЗУЛЬТАТ, А НЕ В ФУНКЦИИ
    printf("Число %u в системе с основанием 2^%u: ", num, r);
    for (int i = subtraction(length, 1); i >= 0; i = subtraction(i, 1)) {
        if (result[i] < 10) {
            printf("%c", summation(result[i], '0'));
        } else {
            printf("%c", summation(result[i], summation('A', -10)));
        }
    }
    printf("\n");

    // само собой память чищу, у меня ВСО, а может и сво...
    free(result);

    return 0;
}
