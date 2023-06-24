#include "stack.h"
#include <stdlib.h>

void stk_init(Stack *p_stack) {
    p_stack->top = NULL;
}

void stk_push(Stack *p_stack, void *p_data){
    Node *p_node = ref_data(p_data);
    p_node->link = p_stack->top;
    p_stack->top = p_node;  
}

int stk_is_empty(Stack *p_stack){
    return p_stack->top == NULL;
}

void *stk_pick(Stack *p_stack) {
    if(p_stack->top != NULL) {
        return p_stack->top->data;
    } else {
        return NULL;
    }
}

void *stk_pop(Stack *p_stack){
    if(p_stack->top != NULL) {
        Node *p_node = p_stack->top;
        p_stack->top = p_node->link;
        return unpack_data(p_node); 
    } else {
        return NULL;
    }
}