#include "stack.h"

void stk_push(Stack *p_stack, void *p_data){
    Node *p_node = ref_data(p_data);
    p_node->link = p_stack->top;
    p_stack->top = p_node;  
}

void *stk_pop(Stack *p_stack){
    Node *p_node = p_stack->top;
    p_stack->top = p_node->link;
    return unpack_data(p_node); 
}