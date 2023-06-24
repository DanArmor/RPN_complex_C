#ifndef _INC_EVAL_H
#define _INC_EVAL_H

#include "list.h"
#include "stack.h"

#include "utils.h"

typedef char TypeName[3];

typedef struct cell {
    TypeName type;
    void *data;
} Cell;

typedef struct variable {
    String name;
    void *value;
} Variable;

typedef struct evaluable_expression {
    List cells;
    List variables;
    List inter_res;
    Stack st;
} EvaluableExpression;

EvaluableExpression evaluable_from_str(char *s);
void read_variables(EvaluableExpression *eval, char **s);
char *calculate_expres(EvaluableExpression *eval);


#endif