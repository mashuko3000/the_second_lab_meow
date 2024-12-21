#include <stdio.h>
#include <stdarg.h>

#define ERR_INVALID_DEGREE -1
#define ERR_UNDEFINED_0POW0 -2 

double fast_pow(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    }

    double result = 1.0;
    unsigned int exp = (exponent < 0) ? -exponent : exponent;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }

    if (exponent < 0) {
        return 1.0 / result; 
    }

    return result;
}

// x - точка, в которой вычисляется многочлен
// degree - степень многочлена
// args - коэффициенты многочлена (от старшей степени к младшей)
int polynomial(double x, int degree, double *result, ...) {
    if (degree < 0) {
        return ERR_INVALID_DEGREE;
    }

    va_list args; 
    va_start(args, result);

    *result = 0.0;

    for (int i = degree; i >= 0; i--) {
        double coeff = va_arg(args, double);
        if (i == 0 && coeff == 0.0 && x == 0.0) {
            va_end(args);
            return ERR_UNDEFINED_0POW0;
        }
        *result += coeff * fast_pow(x, i);
    }

    va_end(args);
    return 0;
}

int main() {
    double x = 2.0;
    int degree = 5;

    double result = 0.0;
    int ret = polynomial(x, degree, &result, 1.98, -6.542, 98.3, 26.54, 67.0, 76.8);

    if (ret == 0) {
        printf("The value of the polynomial at the point x = %.6f: %.6f\n", x, result);
    } else {
        switch (ret) {
            case ERR_INVALID_DEGREE:
                printf("Ошибка: Степень многочлена не может быть отрицательной.\n");
                break;
            case ERR_UNDEFINED_0POW0:
                printf("Ошибка: 0^0 неопределено.\n");
                break;
            default:
                printf("Неизвестная ошибка.\n");
        }
    }

    return 0;
}
