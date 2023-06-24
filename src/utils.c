#include "utils.h"

t_operators_table OPERATORS = {{"(", ")", "-", "~", "+", "*", "/", "ln"},
                               {1, 1, 2, 4, 2, 3, 3, 5}};
t_array_of_numerals_chars NUMERALS = {'0', '1', '2', '3', '4',
                                      '5', '6', '7', '8', '9'};


int operator_num_by_str(char *opr_str){
    int num = 0;
    while(num < NUM_OF_OP && strcmp(opr_str, OPERATORS.operator_str[num]) != 0)
        num += 1;
    if(num == NUM_OF_OP)
        num = -1;
    return num;
}            

static int ch_is_num(char c){
    int i = 0;
    int n = AMOUNT_OF_NUMERALS - 1;
    while(i < n && NUMERALS[i] != c)
        i++;
    return c == NUMERALS[i];
}

int is_const(char *s){
    return ch_is_num(s[0]);
}