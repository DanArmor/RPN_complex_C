#include <stdlib.h>
#include "list.h"

void list_init(List *p_list){
    p_list->start = NULL;
}

int list_is_empty(List *p_list){
    return p_list->start == NULL;
}

Node *list_start(List *p_list){
    return p_list->start;
}

void list_connect_node(List *p_list, Node *p_node) {
    if(p_list->start == NULL){
        p_list->start = p_node;
    } else{
        Node *p_support = list_start(p_list);
        while(p_support->link != NULL)
            p_support = p_support->link;
        p_support->link = p_node;
    }    
}

void list_push_copy(List *p_list, void *p_data, int size) {
    Node *p_node = copy_data(p_data, size);
    list_connect_node(p_list, p_node);    
} 

void list_push_own(List *p_list, void *p_data){
    Node *p_node = own_data(p_data);
    list_connect_node(p_list, p_node);
}

void list_push_ref(List *p_list, void *p_data){
    Node *p_node = ref_data(p_data);
    list_connect_node(p_list, p_node);
}

Node *list_pop_front(List *p_list){
    Node *p_node = p_list->start;
    p_list->start = p_node->link;
    p_node->link = NULL;
    return p_node;
}

void list_clear(List *p_list){
    while(p_list->start != NULL){
        Node *p_node = list_pop_front(p_list);
        destroy_node(p_node);
    }
}

void *list_take_by_index(List *p_list, int index){
    Node *p_node = p_node->link;
    for(int i = 0; i < index; i++){
        p_node = p_node->link;
    }
    return p_node->data;
}

void *list_iter_next(Node **p_iter){
    Node *p_node = *p_iter;
    *p_iter = p_node->link;
    return p_node->data;
}
