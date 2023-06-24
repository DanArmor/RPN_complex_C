#ifndef STACK_H
#define STACK_H
#include "node.h"

typedef struct stack{
    Node *top;
} Stack;

void stk_push(Stack *p_stack, void *p_data);
void *stk_pop(Stack *p_stack);
void stk_init(Stack *p_stack);


#endif