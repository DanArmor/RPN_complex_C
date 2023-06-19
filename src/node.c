#include <stdlib.h>
#include "node.h"

Node *new_node(){
    Node *p_node = (Node*)calloc(1, sizeof(Node));
    p_node->data = NULL;
    p_node->link = NULL;
    return p_node;
}

Node *pack_data(void *p_data){
    Node *p_node = new_node();
    p_node->data = p_data;
    return p_node;
}

void *unpack_data(Node *p_node){
    void *p_data = p_node->data;
    free(p_node);
    return p_data;
}

void destroy_node(Node *p_node) {
    if(p_node->data != NULL) {
        free(p_node->data);
    }
    free(p_node);
}