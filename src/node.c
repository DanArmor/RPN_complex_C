#include <stdlib.h>
#include <string.h>
#include "node.h"

Node *new_node(){
    Node *p_node = (Node*)calloc(1, sizeof(Node));
    p_node->data = NULL;
    p_node->link = NULL;
    return p_node;
}

Node *own_data(void *p_data){
    Node *p_node = new_node();
    p_node->type = NODE_OWN;
    p_node->data = p_data;
    return p_node;
}

Node *copy_data(void *p_data, int size){
    Node *p_node = new_node();
    p_node->type = NODE_OWN;
    p_node->data = (void*)calloc(1, size);
    memcpy(p_node->data, p_data, size);
    return p_node;
}

void *unpack_data(Node *p_node){
    void *p_data = p_node->data;
    free(p_node);
    return p_data;
}

Node *ref_data(void *p_data){
    Node *p_node = new_node();
    p_node->type = NODE_REF;
    p_node->data = p_data;
    return p_node;
}

void destroy_node(Node *p_node) {
    if(p_node->type == NODE_OWN && p_node->data != NULL) {
        free(p_node->data);
    }
    free(p_node);
}