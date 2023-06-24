#include "list.h"
#include "criterion/criterion.h"

Test(list, creation) {
    List p_list;
    list_init(&p_list);
    cr_assert(list_is_empty(&p_list));
}

Test(list, start_empty) {
    List p_list;
    list_init(&p_list);
    cr_assert_null(list_start(&p_list));
}

Test(list, clear_empty) {
    List p_list;
    list_init(&p_list);
    list_clear(&p_list);    
    cr_assert_null(list_start(&p_list));
}

Test(list, start_not_empty) {
    List p_list;
    int a = 3;
    list_init(&p_list);
    list_push_copy(&p_list, &a, sizeof(int));
    cr_assert_not_null(list_start(&p_list));
    list_clear(&p_list);
}

Test(list, clear_one_elem) {
    List p_list;
    int a = 3;
    list_init(&p_list);
    list_push_copy(&p_list, &a, sizeof(int));
    list_clear(&p_list);
    cr_assert_null(list_start(&p_list));
}


Test(list, clear_two_elem) {
    List p_list;
    int a = 3;
    int *b = calloc(1, sizeof(int));
    *b = 4;
    list_init(&p_list);
    list_push_copy(&p_list, &a, sizeof(int));
    list_push_own(&p_list, b);
    list_clear(&p_list);
    cr_assert_null(list_start(&p_list));
}

Test(list, clear_three_elem) {
    List p_list;
    int a = 3;
    int *b = calloc(1, sizeof(int));
    *b = 4;
    list_init(&p_list);
    list_push_copy(&p_list, &a, sizeof(int));
    list_push_own(&p_list, b);
    list_push_ref(&p_list, &a);
    list_clear(&p_list);
    cr_assert_null(list_start(&p_list));
}