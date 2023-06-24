#include "eval.h"

#include <string.h>

extern t_operators_table OPERATORS;
extern t_array_of_numerals_chars NUMERALS;

void init_evaluable_expression(EvaluableExpression *eval) {
    list_init(&eval->cells);
    list_init(&eval->inter_res);
    list_init(&eval->variables);
    stk_init(&eval->st);
}

char *variable_name_from_node(Node *p_node) {
    Variable *p = p_node->data;
    return p->name;
}

int is_variable_in_list(List *list, char *variable_name, int *p_index) {
    Node *p_node = list_start(list);
    *p_index = 0;
    while (p_node != NULL &&
           strcmp(variable_name_from_node(p_node), variable_name) != 0) {
        *p_index += 1;
        p_node = p_node->link;
    }
    return p_node != NULL;
}

char *extract_real_from_str(char *s, int *p_real) {
    int len = strlen(s);
    static char r[256] = "";
    r[0] = '\0';
    int j = 0;
    while (*p_real < len && s[*p_real] != '+' && s[*p_real] != '-') {
        r[j] = s[*p_real];
        j += 1;
        *p_real += 1;
    }
    if (s[*p_real - 1] == 'i') {
        *p_real = 0;
        r[0] = '0';
        r[1] = '\0';
    }
    return r;
}

char *extract_imaginary_from_str(char *s, int *p_imaginary) {
    int len = strlen(s);
    static char r[256] = "";
    r[0] = '\0';
    int j = 0;
    while (*p_imaginary < (len - 1)) {
        r[j] = s[*p_imaginary];
        j += 1;
        *p_imaginary += 1;
    }
    if (r[0] == '\0') {
        r[0] = '0';
        r[1] = '\0';
    }
    return r;
}

// Возвращает истину, если оператор бинарный
int take_operands(Stack *stack, void **p1, void **p2, char *opr_s) {
    if (strcmp(opr_s, "+") == 0 || strcmp(opr_s, "-") == 0 ||
        strcmp(opr_s, "*") == 0 || strcmp(opr_s, "/") == 0) {
        *p1 = stk_pop(stack);
        *p2 = stk_pop(stack);
        return 1;
    } else {
        *p1 = stk_pop(stack);
        return 0;
    }
}

Complex complex_from_str(char *s) {
    int i = 0;
    float re = strtof(extract_real_from_str(s, &i), NULL);
    float im = strtof(extract_imaginary_from_str(s, &i), NULL);
    Complex comp = {re, im};
    return comp;
}

void form_var_cell(Cell *p_cell, char *name) {
    Variable *p_var = (Variable *)calloc(1, sizeof(Variable));
    strcat(p_var->name, name);
    p_cell->data = p_var;
}

void form_con_cell(Cell *p_cell, char *s) {
    Complex *p_complex = (Complex *)calloc(1, sizeof(Complex));
    *p_complex = complex_from_str(s);
    p_cell->data = p_complex;
}

void form_opr_cell(Cell *p_cell, char *s) {
    p_cell->data = &OPERATORS.operator_str[operator_num_by_str(s)];
}

void create_data_cell(Cell *p_cell, char *s) {
    if (strcmp(p_cell->type, "var") == 0) {
        form_var_cell(p_cell, s);
    } else if (strcmp(p_cell->type, "con") == 0) {
        form_con_cell(p_cell, s);
    } else if (strcmp(p_cell->type, "opr") == 0) {
        form_opr_cell(p_cell, s);
    }
}

char *cell_type(char *s) {
    static char r[256] = "";
    r[0] = '\0';
    if (operator_num_by_str(s) == -1) {
        if (is_const(s)) {
            strcat(r, "con");
        } else {
            strcat(r, "var");
        }
    } else {
        strcat(r, "opr");
    }
    return r;
}

void add_link_to_variable(Variable *p_main, Variable *p_var) {
    p_var->value = p_main->value;
}

Variable *create_variable_main(Variable *p_var) {
    Variable *p_main = (Variable *)calloc(1, sizeof(Variable));
    Complex *p_complex = (Complex *)calloc(1, sizeof(Complex));
    p_main->value = p_complex;
    p_var->value = p_complex;
    p_main->name[0] = '\0';
    strcat(p_main->name, p_var->name);
    return p_main;
}

void update_list_of_variables(List *list, Variable *p_var) {
    int i = 0;
    Variable *p_main;
    if (is_variable_in_list(list, p_var->name, &i)) {
        p_main = list_take_by_index(list, i);
        add_link_to_variable(p_main, p_var);
    } else {
        p_main = create_variable_main(p_var);
        list_push_own(list, p_main);
    }
}

EvaluableExpression evaluable_from_str(char *s) {
    EvaluableExpression eval;
    init_evaluable_expression(&eval);
    int j = 0;
    int len = count_words(s);
    int i = 0;
    for (i = 0; i < len; i++) {
        char *part = get_word(s, &j);
        Cell *cell = (Cell *)calloc(1, sizeof(cell));
        strcat(cell->type, cell_type(part));
        create_data_cell(cell, part);
        list_push_own(&eval.cells, cell);
        if (strcmp(cell->type, "var") == 0)
            update_list_of_variables(&eval.variables, cell->data);
    }
    return eval;
}

void read_variables(EvaluableExpression *eval, char **s) {
    Node *p_node = list_start(&eval->variables);
    do {
        Variable *p = list_iter_next(&p_node);
        Complex *p_complex = p->value;
        *p_complex = complex_from_str(*s);
        s++;
    } while (p_node != NULL);
}

void process_operator(EvaluableExpression *eval, char *operator_string) {
    Complex *pr;
    void *p1 = NULL, *p2 = NULL;
    pr = (Complex *)calloc(1, sizeof(Complex));
    if(take_operands(&eval->st, &p1, &p2, operator_string)) {
        *pr = take_binary_operator(operator_num_by_str(operator_string))(*(Complex *)p1, *(Complex *)p2);
    } else {
        *pr = take_unary_operator(operator_num_by_str(operator_string))(*(Complex *)p1);
    }
    stk_push(&eval->st, pr);
    list_push_own(&eval->inter_res, pr);
}

void *take_var_value_from_cell(Cell *cell) {
    Variable *p_var = cell->data;
    return p_var->value;
}

void process_cell(EvaluableExpression *eval, Cell *cell) {
    if (strcmp(cell->type, "opr") == 0)
        process_operator(eval, cell->data);
    else if (strcmp(cell->type, "con") == 0)
        stk_push(&eval->st, cell->data);
    else if (strcmp(cell->type, "var") == 0)
        stk_push(&eval->st, take_var_value_from_cell(cell));
}

char *calculate_expres(EvaluableExpression *eval) {
    static char r[1024] = "";
    r[0] = '\0';
    Node *p_node = (Node *)calloc(1, sizeof(Node));
    *p_node = *list_start(&eval->cells);
    do {
        Cell *p_cell = list_iter_next(&p_node);
        process_cell(eval, p_cell);
    } while (p_node != NULL);
    strcat(r, complex_to_string(eval->st.top->data));
    list_clear(&eval->inter_res);
    return r;
}

int is_have_vars(EvaluableExpression *eval) {
    return !list_is_empty(&eval->variables);
}