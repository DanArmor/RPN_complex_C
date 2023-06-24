#ifndef LIST_H
#define LIST_H
#include "node.h"

typedef struct list{
    Node *start;
} List;

void list_init(List *p_list);
int list_is_empty(List *p_list);
Node *list_start(List *p_list);

void list_push_own(List *p_list, void *p_data);
void list_push_copy(List *p_list, void *p_data, int size);
void list_push_ref(List *p_list, void *p_data);

Node *list_pop_front(List *p_list);
void list_clear(List *p_list);
void *list_take_by_index(List *p_list, int num);
// Возвращает элемент в p_iter и перемещает итератор на следующий узел
void *list_iter_next(Node **p_iter);

#endif