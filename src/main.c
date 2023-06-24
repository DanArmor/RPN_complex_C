#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "node.h"
#include "rpn.h"
#include "stack.h"
#include "utils.h"
#include "eval.h"

int main(void) {
    char s[256] = "";
    fgets(s, 256, stdin);
    RPN_express rpn = rpn_from_str(s);
    puts(rpn.expression);
    EvaluableExpression RPN = evaluable_from_str(rpn.expression);
    if (is_have_vars(&RPN)) {
        while (1) {
            read_variables(&RPN, NULL);
            puts(calculate_expres(&RPN));
        }
    } else {
        puts(calculate_expres(&RPN));
    }
    return 0;
}