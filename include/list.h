#ifndef LIST_H
#define LIST_H
#include "node.h"

typedef struct list{
    Node *start;
} List;

void list_init(List *p_list);
int list_is_empty(List *p_list);
Node *lst_take_start(List *p_list);
void lst_push(List *p_list, void *p_data);
void *lst_pop_first(List *p_list);
void lst_clear(List *p_list);
void *lst_take_by_num(List *p_list, int num);

#endif