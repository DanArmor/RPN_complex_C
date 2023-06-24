#ifndef _INC_UTILS_H
#define _INC_UTILS_H

#include <stdlib.h>

typedef char String[256];

#define NUM_OF_OP 8
#define MAX_LENGTH_OF_OPERATOR 3
#define MAX_VARIABLE_NAME_LENGTH 5
#define AMOUNT_OF_NUMERALS 10

typedef char t_max_str_op[MAX_LENGTH_OF_OPERATOR];
typedef t_max_str_op t_array_of_op_str[NUM_OF_OP];
typedef size_t t_array_of_op_pri[NUM_OF_OP];
typedef char t_array_of_numerals_chars[AMOUNT_OF_NUMERALS];

typedef struct operators_table {
    t_array_of_op_str operator_str;
    t_array_of_op_pri operator_priority;
} t_operators_table;

int operator_num_by_str(char *opr_str);
int is_const(char *s);

#endif