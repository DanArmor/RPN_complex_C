#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "node.h"
#include "rpn.h"
#include "stack.h"
#include "utils.h"
#include "eval.h"

char *fgets_no_new_line(char *s, size_t n, FILE *stream){
    char *result = fgets(s, n, stream);
    result[strcspn(result, "\n")] = '\0';
    return result;
}

#define N 6
#define SIZE 16

int main(void) {

    char expression_str[256] = "";
    char **variables_str = calloc(6, sizeof(char*));
    for(int i = 0; i < N; i++){
        variables_str[i] = calloc(SIZE, sizeof(char));
    }
    fgets(expression_str, 256, stdin);
    rpn_str_from_str(expression_str, expression_str);
    puts(expression_str);
    EvaluableExpression eval = evaluable_from_str(expression_str);
    if (is_have_vars(&eval)) {
        while (1) {
            int size = get_variables_amount(&eval);
            for(int i = 0; i < size; i++){
                fgets_no_new_line(variables_str[i], 32, stdin);
            }
            read_variables(&eval, variables_str);
            puts(calculate_expres(&eval));
        }
    } else {
        puts(calculate_expres(&eval));
    }
    
    for(int i = 0; i < N; i++){
        free(variables_str[i]);
    }
    free(variables_str);

    return 0;
}