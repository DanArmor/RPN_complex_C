#ifndef _INC_RPN_H
#define _INC_RPN_H

#include "stack.h"
#include "utils.h"

typedef struct RPN_express{
    String expres;
    Stack st; 
} RPN_express;

void init_express(RPN_express *expres);
RPN_express RPN_from_str(char *s);

#endif