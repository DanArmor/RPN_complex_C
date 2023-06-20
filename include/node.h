#ifndef NODE_H
#define NODE_H

#define NODE_REF 1
#define NODE_OWN 2

typedef struct Node{
    int type;
    void *data;
    struct Node *link;
} Node;

Node *new_node();
// Создание узла, который контролирует ресурс
Node *own_data(void *p_data);
// Создание узла, в который копируются данные
Node *copy_data(void *p_data, int size);
// Создание узла, который ссылается на ресурс, время жизни которого он не контролирует
Node *ref_data(void *p_data);

int node_is_ref(Node *p_node);
int node_is_own(Node *p_node);

// Уничтожает узел, возвращает указатель на данные
void *unpack_data(Node *p_node);
// Уничтожает узел и данные, ЕСЛИ узел ими владеет
void destroy_node(Node *p_node);

#endif