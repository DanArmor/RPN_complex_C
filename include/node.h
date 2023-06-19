#ifndef NODE_H
#define NODE_H
typedef struct Node{
    void *data;
    struct Node *link;
} Node;

Node *new_node();
Node *pack_data(void *p_data);
void *unpack_data(Node *p_node);
void destroy_node(Node *p_node);

#endif