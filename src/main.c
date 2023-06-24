#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define SQR(x) ((x)*(x))

#include "node.h"     
#include "stack.h"    
#include "list.h"
#include "utils.h"

#include "rpn.h"

typedef struct complex{
    float re;
    float im;
} Complex;

typedef char Tname[3];

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

extern t_operators_table OPERATORS;
extern t_array_of_numerals_chars NUMERALS;

void init_calc_express(Calculated_RPN_express *expres){
    expres->express.start = NULL;
    list_init(&expres->inter_res);
    list_init(&expres->variables);
    stk_init(&expres->st);
}

char *take_variable_name_from_node(Node *p_node){
    Variable *p = p_node->data;
    return p->name;
}

int is_variable_in_list(List *list, char *name, int *p_i){
    Node *p_node = list_start(list);
    *p_i = 0;
    while(p_node != NULL && strcmp(take_variable_name_from_node(p_node), name) != 0){
        *p_i += 1;
        p_node = p_node->link;
    }
    return p_node != NULL;
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
    p_cell->data = &OPERATORS.operator_str[operator_num_by_str(s)];
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
    if(operator_num_by_str(s) == -1){
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
        p_main = list_take_by_index(list, i);
        add_link_to_variable(p_main, p_var);
    } else{
        p_main = create_variable_main(p_var);
        list_push_own(list, p_main);
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
        list_push_own(&expres.express, cell);
        if(strcmp(cell->type, "var") == 0)
            update_list_of_variables(&expres.variables, cell->data);
    }
    return expres;
}

void read_variables(Calculated_RPN_express *expres){
    Node *p_node = list_start(&expres->variables);
    do{
        Variable *p = list_iter_next(&p_node);
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
    list_push_own(&expres->inter_res, pr);
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
    *p_node = *list_start(&expres->express);
    do{
        Cell *p_cell = list_iter_next(&p_node);
        process_cell(expres, p_cell);
    } while (p_node != NULL);
    strcat(r, complex_to_string(expres->st.top->data));
    list_clear(&expres->inter_res);
    return r;
}

int is_have_vars(Calculated_RPN_express *expres){
    return expres->variables.start != NULL;
}

int main(void){
    char s[256] = "";
    fgets(s, 256, stdin);
    RPN_express rpn = rpn_from_str(s);
    puts(rpn.expression);
    Calculated_RPN_express RPN = string_into_calculated(rpn.expression);
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