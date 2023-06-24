#ifndef _INC_UTILS_H
#define _INC_UTILS_H

#include <stdlib.h>

typedef char String[256];

#define NUM_OF_OP 8
#define MAX_LENGTH_OF_OPERATOR 3
#define MAX_VARIABLE_NAME_LENGTH 5
#define AMOUNT_OF_NUMERALS 10

typedef struct complex {
    float re;
    float im;
} Complex;

typedef Complex (*t_operators_binary_func)(Complex, Complex);
typedef Complex (*t_operators_unary_func)(Complex);
typedef void *t_operators_func;

typedef char t_max_str_op[MAX_LENGTH_OF_OPERATOR];
typedef t_max_str_op t_array_of_op_str[NUM_OF_OP];
typedef size_t t_array_of_op_pri[NUM_OF_OP];
typedef t_operators_func t_array_of_operators_func[NUM_OF_OP];
typedef char t_array_of_numerals_chars[AMOUNT_OF_NUMERALS];

typedef struct operators_table {
    t_array_of_op_str operator_str;
    t_array_of_op_pri operator_priority;
    t_array_of_operators_func operator_func;
} t_operators_table;

int operator_num_by_str(char *opr_str);
int is_const(char *s);

Complex perfome_binary_operation(Complex a, Complex b,
                                 Complex operation(Complex, Complex));

Complex perfome_unary_operation(Complex a, Complex operation(Complex));

t_operators_binary_func take_binary_operator(int operator_num);

t_operators_unary_func take_unary_operator(int operator_num);

char *complex_to_string(Complex *p_complex);

char *get_word(char *s, int *word_begin_index);

int count_words(char *s);

#endif