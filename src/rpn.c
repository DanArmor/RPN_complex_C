#include "rpn.h"

#include <stdlib.h>
#include <string.h>

extern t_operators_table OPERATORS;
extern t_array_of_numerals_chars NUMERALS;

// Сравнивает приоритеты операторов по их номерам
static int operator_priority_compare(int left_operator_number,
                                     int right_operator_number) {
    return OPERATORS.operator_priority[left_operator_number] >=
           OPERATORS.operator_priority[right_operator_number];
}

// Возвращает номер унарной версии оператора, если он унарный
static int binary_to_unary(int operator_number) {
    if (strcmp(OPERATORS.operator_str[operator_number], "-") == 0) {
        return operator_number + 1;
    }
    return operator_number;
}

void init_rpn(RPN_express *rpn) {
    rpn->expression[0] = '\0';
    stk_init(&rpn->stack);
}

static void pop_operators(RPN_express *rpn, int operator_number) {
    if (stk_is_empty(&rpn->stack)) {
        return;
    }
    while (!stk_is_empty(&rpn->stack) &&
           operator_priority_compare(operator_num_by_str(stk_pick(&rpn->stack)),
                                     operator_number)) {
        char *operator_str = stk_pop(&rpn->stack);
        strcat(rpn->expression, operator_str);
        strcat(rpn->expression, " ");
    }
}

static void end_bracket_pushing(RPN_express *rpn) {
    while (strcmp(stk_pick(&rpn->stack), "(") != 0) {
        void *pt = stk_pop(&rpn->stack);
        strcat(rpn->expression, pt);
        strcat(rpn->expression, " ");
    }
    stk_pop(&rpn->stack);
}

static void update_expression_operators(RPN_express *rpn, int operator_number,
                                        int is_unary) {
    if (is_unary) {
        operator_number = binary_to_unary(operator_number);
    }
    char *operator_str = OPERATORS.operator_str[operator_number];
    if (strcmp(operator_str, "(") == 0) {
        stk_push(&rpn->stack, operator_str);
    } else if (strcmp(operator_str, ")") == 0) {
        end_bracket_pushing(rpn);
    } else {
        if (!stk_is_empty(&rpn->stack.top)) {
            pop_operators(rpn, operator_number);
        }
        stk_push(&rpn->stack, operator_str);
    }
}

static void pop_stack_of_expres(RPN_express *rpn) {
    while (!stk_is_empty(&rpn->stack)) {
        char *operator_str = stk_pop(&rpn->stack);
        strcat(rpn->expression, operator_str);
        strcat(rpn->expression, " ");
    }
}

static void str_into_rpn(RPN_express *rpn, char *s) {
    init_rpn(rpn);
    int startOfWord = 0;
    int is_unary = 1;
    char *word = get_word(s, &startOfWord);
    while (*word != '\0') {
        int operator_number = operator_num_by_str(word);
        if (operator_number == -1) {
            strcat(rpn->expression, word);
            strcat(rpn->expression, " ");
        } else {
            update_expression_operators(rpn, operator_number, is_unary);
        }
        is_unary = operator_number != -1;
        word = get_word(s, &startOfWord);
    }
    pop_stack_of_expres(rpn);
}

RPN_express rpn_from_str(char *s) {
    RPN_express rpn;
    init_rpn(&rpn);
    str_into_rpn(&rpn, s);
    return rpn;
}

void rpn_str_from_str(char *dst, char *src){
    RPN_express rpn = rpn_from_str(src);
    strcpy(dst, rpn.expression);
}