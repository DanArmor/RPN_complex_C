#include "list.h"
#include "criterion/criterion.h"

Test(list, creation) {
    List p_list;
    list_init(&p_list);
    cr_assert(list_is_empty(&p_list));
}