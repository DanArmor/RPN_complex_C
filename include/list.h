#ifndef LIST_H
#define LIST_H
#include "node.h"

typedef struct list{
    Node *start;
} List;

void list_init(List *p_list);
int list_is_empty(List *p_list);
Node *list_start(List *p_list);
void list_push(List *p_list, void *p_data);
void *list_pop_front(List *p_list);
void list_clear(List *p_list);
void *list_take_by_index(List *p_list, int num);

#endif