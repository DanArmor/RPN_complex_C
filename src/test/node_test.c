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

Test(node, copy_data) {}