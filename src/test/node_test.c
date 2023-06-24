#include <signal.h>

#include "node.h"


#include "criterion/criterion.h"

Test(node, creation) {
    Node *p_node = new_node();
    cr_assert_null(p_node->data);
    cr_assert_null(p_node->link);
}

Test(node, ref_data) {
    int a = 3;
    Node *p_node = ref_data(&a);
    cr_assert(p_node->type == NODE_REF);
    cr_assert(*(int *)(p_node->data) == a);
    destroy_node(p_node);
}

Test(node, copy_data) {
    int a = 3;
    Node *p_node = copy_data(&a, sizeof(int));
    cr_assert(p_node->type == NODE_OWN);
    cr_assert(*(int *)(p_node->data) == a);
    destroy_node(p_node);
}

Test(node, own_data) {
    int *a = calloc(1, sizeof(int));
    *a = 3;
    Node *p_node = own_data(a);
    cr_assert(p_node->type == NODE_OWN);
    cr_assert(*(int *)(p_node->data) == *a);
    destroy_node(p_node);
}
