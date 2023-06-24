#ifndef _INC_RPN_H
#define _INC_RPN_H

#include "stack.h"
#include "utils.h"

typedef struct RPN_express{
    String expression;
    Stack stack; 
} RPN_express;

void init_rpn(RPN_express *expres);
RPN_express rpn_from_str(char *s);

#endif