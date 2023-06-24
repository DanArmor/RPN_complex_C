#include "RPN.h"

#include <stdlib.h>

static char *get_word(char *s, int *p_i){
    static char buff[1024] = "";
    int len = strlen(s);
    while(s[*p_i] <= ' ' && *p_i < len)
        *p_i += 1;
    buff[0] = '\0';
    int j = 0;
    while(s[*p_i] > ' ' && *p_i < len){
        buff[j] = s[*p_i];
        *p_i += 1;
        j += 1;
    }
    buff[j] = '\0';
    return buff;
}

static int count_words(char *s){
    int n = 0;
    int i = 0;
    while(strlen(get_word(s, &i)) != 0)
        n += 1;
    return n;
}

void init_express(RPN_express *expres){
    expres->expres[0] = '\0';
    stk_init(&expres->st);
}

static void pop_operators(RPN_express *expres, int op_n){
    char *p_opr = expres->st.top->data;
    Node *p_head = expres->st.top;
    while(p_head != NULL && pri_comp(num_of_operator(p_opr), op_n)){
        strcat(expres->expres, p_opr);
        strcat(expres->expres, " ");
        expres->st.top = p_head->link;
        free(p_head);
        p_head = expres->st.top;
        if(p_head != NULL)
            p_opr = expres->st.top->data;
    }
}

static void end_bracket_pushing(RPN_express *expres){
    while(strcmp(expres->st.top->data, "(") != 0){
        void *pt = stk_pop(&expres->st);
        strcat(expres->expres, pt);
        strcat(expres->expres, " ");
    }
    stk_pop(&expres->st);
}

static int pri_comp(int i, int j){
    return OPERATORS.op_p[i] >= OPERATORS.op_p[j];
}

static int binary_to_unary(int op_n){
    if(strcmp(OPERATORS.op_s[op_n], "-") == 0)
        return op_n + 1;
    return op_n;
}

static void update_expression_operators(RPN_express *expres, int op_n, int is_un){
    if(is_un)
        op_n = binary_to_unary(op_n);
    char *p = OPERATORS.op_s[op_n];
    if(strcmp(p, "(") == 0){
        stk_push(&expres->st, p);
    } else if(strcmp(p, ")") == 0){
        end_bracket_pushing(expres);
    } else{
        if(expres->st.top != NULL)
            pop_operators(expres, op_n);
        stk_push(&expres->st, p);
    }
}

static void pop_stack_of_expres(RPN_express *expres){
    while(expres->st.top != NULL){
        char *p_opr = stk_pop(&expres->st);
        strcat(expres->expres, p_opr);
        strcat(expres->expres, " ");
    }
}

static void trans_expression_to_RPN(RPN_express *expres, char *s){
    init_express(expres);
    int i = 0;
    int is_un = 1;
    char *part = get_word(s, &i);
    while(*part != '\0'){
        int num_op = num_of_operator(part);
        if(num_op == -1){
            strcat(expres->expres, part);
            strcat(expres->expres, " ");
        } else{
            update_expression_operators(expres, num_op, is_un);
        }
        is_un = num_op != -1;
        part = get_word(s, &i);
    }
    pop_stack_of_expres(expres);
}

RPN_express RPN_from_str(char *s){
    RPN_express expres;
    init_express(&expres);
    trans_expression_to_RPN(&expres, s);
    return expres;
}