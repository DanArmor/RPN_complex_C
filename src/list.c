#include <stdlib.h>
#include "list.h"

void list_init(List *p_list){
    p_list->start = NULL;
}

int list_is_empty(List *p_list){
    return p_list->start == NULL;
}

Node *list_take_start(List *p_list){
    return p_list->start;
}

void list_push(List *p_list, void *p_data){
    Node *p_node = pack_data(p_data);
    if(p_list->start == NULL){
        p_list->start = p_node;
    } else{
        Node *p_support = list_take_start(p_list);
        while(p_support->link != NULL)
            p_support = p_support->link;
        p_support->link = p_node;
    }
}

void *list_pop_first(List *p_list){
    Node *p_node = p_list->start;
    p_list->start = p_node->link;
    return unpack_data(p_node);
}

void list_clear(List *p_list){
    while(p_list->start != NULL)
        free(list_pop_first(p_list));
}

void *list_take_by_num(List *p_list, int num){
    int i = 0;
    Node *p_node = p_list->start;
    while(i < num){
        p_node = p_node->link;
        i++;
    }
    return p_node->data;
}