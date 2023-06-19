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
// Создание узла, который ссылается на ресурс, время жизни которого он не контролирует
Node *ref_data(void *p_data);

void *unpack_data(Node *p_node);
// Нельзя применять на узлах, которые не владеют значением внутри себя
void destroy_node(Node *p_node);

#endif