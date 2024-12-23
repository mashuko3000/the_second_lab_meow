#include <stdio.h>
#include <stdlib.h>

#define PTR_ERROR -1
#define OK 0
#define INVALID_INPUT 3
#define MEMORY_ERROR -2

int sums_decomposition(int value, int ***result_decompositions, size_t *result_decompositions_count, int allowed_equal_sum_components);
void find_decompositions(int value, int *current_decomposition, int current_size, int **decompositions, size_t *count, int allowed_equal_sum_components);

int main(){
    int value = 10;
    int **decompositions = NULL;
    size_t decompositions_count = 0;
    int allowed_equal_sum_components = 1;

    int result = sums_decomposition(value, &decompositions, &decompositions_count, allowed_equal_sum_components);

    if (result == 0) {
        printf("Разложения числа %d на суммы (с флагом %d):\n", value, allowed_equal_sum_components);
        for (size_t i = 0; i < decompositions_count; i++) {
            printf("%zu ", i + 1);
            printf("%d ", decompositions[i][0]);
            for (int j = 1; j <= decompositions[i][0]; j++) {
                printf("%d ", decompositions[i][j]);  
            }
            printf("\n");
            free(decompositions[i]); 
        }
        free(decompositions);
    } else {
        printf("Произошла ошибка с кодом: %d\n", result);
    }

    return 0;

}

void find_decompositions(int value, int *current_decomposition, int current_size, int **decompositions, size_t *count, int allowed_equal_sum_components) {
    if (value == 0){
        decompositions[*count] = (int*)malloc(sizeof(int)*(current_size + 1));
        if (decompositions[*count] == NULL){
            return;
        }
        int i = 0;
        decompositions[*count][0] = current_size; // im adding count of decomposition's elements to zero index
        for (i; i < current_size; i++){
            decompositions[*count][i+1] = current_decomposition[i]; 
        }
        (*count)++;
        return;
    }

    int j = ((current_size == 0 ? 1 : (allowed_equal_sum_components == 1 ? current_decomposition[current_size - 1] : current_decomposition[current_size - 1] + 1)));
    for (j; j<=value; j++){
        current_decomposition[current_size] = j;
        find_decompositions(value-j, current_decomposition, current_size+1, decompositions, count, allowed_equal_sum_components);
    }
}

int sums_decomposition(int value, int ***result_decompositions, size_t *result_decompositions_count, int allowed_equal_sum_components)
{
    //checking pointers and validation of value
    if (result_decompositions == NULL || result_decompositions_count == NULL){
        return PTR_ERROR;
    }
    if (value <= 0){
        return PTR_ERROR;
    }
    *result_decompositions = NULL;
    *result_decompositions_count = 0;

    //memory allocation
    int ** decompositions = (int**)malloc(500*sizeof(int*));
    if (decompositions == NULL){
        return MEMORY_ERROR;
    }

    int *current_decomposition = (int *)malloc(value * sizeof(int));
    if (current_decomposition == NULL) {
        free(decompositions);
        return PTR_ERROR;
    }

    // find decompositions by new function
    find_decompositions(value, current_decomposition, 0, decompositions, result_decompositions_count, allowed_equal_sum_components);

    free(current_decomposition);

    int **temp = (int **)realloc(decompositions, (*result_decompositions_count) * sizeof(int *));
    if (temp == NULL) {
        free(decompositions);
        return 4;
    }
    *result_decompositions = temp; 

    return OK;
}

