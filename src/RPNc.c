#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SQR(x) ((x)*(x))

#include "node.h"     
#include "stack.h"    
#include "list.h"
#include "selector.h" 

typedef struct complex{
    float re;
    float im;
} Complex;

typedef char String[256];
typedef char Tname[3];

typedef struct RPN_express{
    String expres;
    Stack st; 
} RPN_express;

typedef struct cell{
    Tname type;
    void *data;
} Cell;

typedef struct variable{
    String name;
    void *value;
} Variable;

typedef struct calculated_RPN_express{
    List express;
    List variables;
    List inter_res;
    Stack st;
} Calculated_RPN_express;

#define NUM_OF_OP 8                
#define MAX_LENGTH_OF_OPERATOR 3   
#define MAX_VARIABLE_NAME_LENGTH 5 
#define AMOUNT_OF_NUMERALS 10

typedef char t_max_str_op[MAX_LENGTH_OF_OPERATOR];
typedef t_max_str_op t_array_of_op_str[NUM_OF_OP];
typedef size_t t_array_of_op_pri[NUM_OF_OP];
typedef char t_array_of_numerals_chars[AMOUNT_OF_NUMERALS];

typedef struct operators_table{
    t_array_of_op_str op_s;
    t_array_of_op_pri op_p;
} t_operators_table;

t_operators_table OPERATORS = {{"(", ")", "-", "~", "+", "*", "/", "ln"},{1, 1, 2, 4, 2, 3, 3, 5}};
t_array_of_numerals_chars NUMERALS = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

int num_of_operator(char *opr_str){
    int num = 0;
    while(num < NUM_OF_OP && strcmp(opr_str, OPERATORS.op_s[num]) != 0)
        num += 1;
    if(num == NUM_OF_OP)
        num = -1;
    return num;
}

char *get_word(char *s, int *p_i){
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

int ch_is_num(char c){
    int i = 0;
    int n = AMOUNT_OF_NUMERALS - 1;
    while(i < n && NUMERALS[i] != c)
        i++;
    return c == NUMERALS[i];
}

int is_const(char *s){
    return ch_is_num(s[0]);
}

int pri_comp(int i, int j){
    return OPERATORS.op_p[i] >= OPERATORS.op_p[j];
}

void init_express(RPN_express *expres){
    expres->expres[0] = '\0';
    expres->st.top = NULL;
}

void init_calc_express(Calculated_RPN_express *expres){
    expres->express.start = NULL;
    expres->inter_res.start = NULL;
    expres->st.top = NULL;
    expres->variables.start = NULL;
}

void pop_operators(RPN_express *expres, int op_n){
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

void end_bracket_pushing(RPN_express *expres){
    while(strcmp(expres->st.top->data, "(") != 0){
        void *pt = stk_pop(&expres->st);
        strcat(expres->expres, pt);
        strcat(expres->expres, " ");
    }
    stk_pop(&expres->st);
}

int binary_to_unary(int op_n){
    if(strcmp(OPERATORS.op_s[op_n], "-") == 0)
        return op_n + 1;
    return op_n;
}

void update_expression_operators(RPN_express *expres, int op_n, int is_un){
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

void pop_stack_of_expres(RPN_express *expres){
    while(expres->st.top != NULL){
        char *p_opr = stk_pop(&expres->st);
        strcat(expres->expres, p_opr);
        strcat(expres->expres, " ");
    }
}

void trans_expression_to_RPN(RPN_express *expres, char *s){
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

char *take_variable_name_from_node(Node *p_node){
    Variable *p = p_node->data;
    return p->name;
}

int is_variable_in_list(List *list, char *name, int *p_i){
    Node *p_node = lst_take_start(list);
    *p_i = 0;
    while(p_node != NULL && strcmp(take_variable_name_from_node(p_node), name) != 0){
        *p_i += 1;
        p_node = p_node->link;
    }
    return p_node != NULL;
}

int count_words(char *s){
    int n = 0;
    int i = 0;
    while(strlen(get_word(s, &i)) != 0)
        n += 1;
    return n;
}

char *take_real_part(char *s, int *p_i){
    int len = strlen(s);
    static char r[256] = "";
    r[0] = '\0';
    int j = 0;
    while(*p_i < len && s[*p_i] != '+' && s[*p_i] != '-'){
        r[j] = s[*p_i];
        j += 1;
        *p_i += 1;
    }
    if(s[*p_i - 1] == 'i'){
        *p_i = 0;
        r[0] = '0';
        r[1] = '\0';
    }
    return r;
}

char *take_imaginary_part(char *s, int *p_i){
    int len = strlen(s);
    static char r[256] = "";
    r[0] = '\0';
    int j = 0;
    while(*p_i < (len - 1)){
        r[j] = s[*p_i];
        j += 1;
        *p_i += 1;
    }
    if(r[0] == '\0'){
        r[0] = '0';
        r[1] = '\0';
    }
    return r;
}

Complex construct_complex(char *s){
    int i = 0;
    float re = strtof(take_real_part(s, &i), NULL);
    float im = strtof(take_imaginary_part(s, &i), NULL);
    Complex comp = {re, im};
    return comp;
}

void form_var_cell(Cell *p_cell, char *name){
    Variable *p_var = (Variable*)calloc(1, sizeof(Variable));
    strcat(p_var->name, name);
    p_cell->data = p_var;
}

void form_con_cell(Cell *p_cell, char *s){
    Complex *p_complex = (Complex*)calloc(1, sizeof(Complex));
    *p_complex = construct_complex(s);
    p_cell->data = p_complex;
}

void form_opr_cell(Cell *p_cell, char *s){
    p_cell->data = &OPERATORS.op_s[num_of_operator(s)];
}

void create_data_cell(Cell *p_cell, char *s){
    if(strcmp(p_cell->type, "var") == 0){
        form_var_cell(p_cell, s);
    } else if(strcmp(p_cell->type, "con") == 0){
        form_con_cell(p_cell, s);
    } else if(strcmp(p_cell->type, "opr") == 0){
        form_opr_cell(p_cell, s);
    }
}

char *cell_type(char *s){
    static char r[256] = "";
    r[0] = '\0';
    if(num_of_operator(s) == -1){
        if(is_const(s)){
            strcat(r, "con");
        } else{
            strcat(r, "var");
        }
    } else{
        strcat(r, "opr");
    }
    return r;
}

void add_link_to_variable(Variable *p_main, Variable *p_var){
    p_var->value = p_main->value;
}

Variable *create_variable_main(Variable *p_var){
    Variable *p_main = (Variable*)calloc(1, sizeof(Variable));
    Complex *p_complex = (Complex*)calloc(1, sizeof(Complex));
    p_main->value = p_complex;
    p_var->value = p_complex;
    p_main->name[0] = '\0';
    strcat(p_main->name, p_var->name);
    return p_main;
}

void update_list_of_variables(List *list, Variable *p_var){
    int i = 0;
    Variable *p_main;
    if(is_variable_in_list(list, p_var->name, &i)){
        p_main = lst_take_by_num(list, i);
        add_link_to_variable(p_main, p_var);
    } else{
        p_main = create_variable_main(p_var);
        lst_push(list, p_main);
    }
}

Calculated_RPN_express string_into_calculated(char *s){
    Calculated_RPN_express expres;
    init_calc_express(&expres);
    int j = 0;
    int len = count_words(s);
    int i = 0;
    for (i = 0; i < len; i++) {
        char *part = get_word(s, &j);
        Cell *cell = (Cell*)calloc(1, sizeof(cell));
        strcat(cell->type, cell_type(part));
        create_data_cell(cell, part);
        lst_push(&expres.express, cell);
        if(strcmp(cell->type, "var") == 0)
            update_list_of_variables(&expres.variables, cell->data);
    }
    return expres;
}

void read_variables(Calculated_RPN_express *expres){
    Node *p_node = lst_take_start(&expres->variables);
    do{
        Variable *p = slctr_take(&p_node);
        printf("Enter values of %s: ", p->name);
        char s[256];
        fgets(s, 256, stdin);
        Complex *p_complex = p->value;
        *p_complex = construct_complex(s);
    } while (p_node != NULL);
}

Complex addition(Complex a, Complex b){
    Complex r = {a.re + b.re, a.im + b.im};
    return r;
}

Complex substraction(Complex a, Complex b){
    Complex r = {a.re - b.re, a.im - b.im};
    return r;
}

Complex multiplication(Complex a, Complex b){
    Complex r = {(a.re * b.re - a.im * b.im), (a.re * b.im + a.im * b.re)};
    return r;
}

Complex division(Complex a, Complex b){
    Complex r = {(a.re * b.re + a.im * b.im) / (SQR(a.re) + SQR(a.im)), (a.re * b.im - a.im * b.re) / (SQR(a.re) + SQR(a.im))};
    return r;
}

Complex unary_minus(Complex a){
    Complex r = {a.re * -1, a.im * -1};
    return r;
}

Complex natural_log(Complex a){
    Complex r = {logf(sqrtf(SQR(a.re) + SQR(a.im))), atanf(a.im/a.re)};
    return r;
}

void take_operands(Stack *stack, void **p1, void **p2, char *opr_s){
    if(strcmp(opr_s, "+") == 0 || strcmp(opr_s, "-") == 0 || strcmp(opr_s, "*") == 0 || strcmp(opr_s, "/") == 0){
        *p1 = stk_pop(stack);
        *p2 = stk_pop(stack);
    } else{
        *p1 = stk_pop(stack); 
    }
}

char *complex_to_string(void *p){
    Complex a = *(Complex*)p;
    static char r[256] = "";
    static char buff[256] = "";
    r[0] = '\0';
    a.im = fabsf(a.im);
    sprintf(buff, "%.2f%c", a.re, '\0');
    strcat(r,  buff);
    sprintf(buff, "%+.2f%c%c", a.im, 'i', '\0');
    strcat(r,  buff);
    return r;
}

void process_operator(Calculated_RPN_express *expres, char *opr_s){
    Complex *pr;
    void *p1 = NULL, *p2 = NULL;
    pr = (Complex*)calloc(1, sizeof(Complex));
    take_operands(&expres->st, &p1, &p2, opr_s);
    if(strcmp(opr_s, "+") == 0)
        *pr = addition(*(Complex*)p1, *(Complex*)p2);
    else if(strcmp(opr_s, "-") == 0)
        *pr = substraction(*(Complex*)p1, *(Complex*)p2);
    else if(strcmp(opr_s, "*") == 0)
        *pr = multiplication(*(Complex*)p1, *(Complex*)p2);
    else if(strcmp(opr_s, "/") == 0)
        *pr = division(*(Complex*)p1, *(Complex*)p2);
    else if(strcmp(opr_s, "~") == 0)
        *pr = unary_minus(*(Complex*)p1);
    else if(strcmp(opr_s, "ln") == 0)
        *pr = natural_log(*(Complex*)p1);
    stk_push(&expres->st, pr);
    lst_push(&expres->inter_res, pr);
}

void *take_var_value_from_cell(Cell *cell){
    Variable *p_var = cell->data;
    return p_var->value; 
}

void process_cell(Calculated_RPN_express *expres, Cell *cell){
    if(strcmp(cell->type, "opr") == 0)
        process_operator(expres, cell->data);
    else if(strcmp(cell->type, "con") == 0)
        stk_push(&expres->st, cell->data);
    else if(strcmp(cell->type, "var") == 0)
        stk_push(&expres->st, take_var_value_from_cell(cell));
}

char *calculate_expres(Calculated_RPN_express *expres){
    static char r[1024] = "";
    r[0] = '\0';
    Node *p_node = (Node*)calloc(1, sizeof(Node));
    *p_node = *lst_take_start(&expres->express);
    do{
        Cell *p_cell = slctr_take(&p_node);
        process_cell(expres, p_cell);
    } while (p_node != NULL);
    strcat(r, complex_to_string(expres->st.top->data));
    lst_clear(&expres->inter_res);
    return r;
}

int is_have_vars(Calculated_RPN_express *expres){
    return expres->variables.start != NULL;
}

int main(void){
    RPN_express expres;
    char s[256] = "";
    fgets(s, 256, stdin);
    trans_expression_to_RPN(&expres, s);
    puts(expres.expres);
    Calculated_RPN_express RPN = string_into_calculated(expres.expres);
    if(is_have_vars(&RPN)){
        while(1){
            read_variables(&RPN);
            puts(calculate_expres(&RPN));
        }
    } else{
        puts(calculate_expres(&RPN));
    }
    return 0;
}