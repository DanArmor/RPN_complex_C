#include <stdlib.h>
#include "head/node.h"

Node *new_node(){
    return (Node*)calloc(1, sizeof(Node));
}

Node *pack_data(void *p_data){
    Node *p_node = new_node();
    p_node->data = p_data;
    p_node->link = NULL;
    return p_node;
}

void *unpack_data(Node *p_node){
    void *p_data = p_node->data;
    free(p_node);
    return p_data;
}