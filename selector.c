#include <stdlib.h>
#include "head/selector.h" //FIXME: HEADER

void *slctr_take(Node **p_selector){
    Node *p_node = *p_selector;
    *p_selector = p_node->link;
    return p_node->data;
}
