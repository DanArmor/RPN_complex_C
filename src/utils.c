#include "utils.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

float SQR(float x) { return x * x; }

static Complex addition(Complex a, Complex b) {
    Complex r = {a.re + b.re, a.im + b.im};
    return r;
}

static Complex substraction(Complex a, Complex b) {
    Complex r = {a.re - b.re, a.im - b.im};
    return r;
}

static Complex multiplication(Complex a, Complex b) {
    Complex r = {(a.re * b.re - a.im * b.im), (a.re * b.im + a.im * b.re)};
    return r;
}

static Complex division(Complex a, Complex b) {
    Complex r = {(a.re * b.re + a.im * b.im) / (SQR(a.re) + SQR(a.im)),
                 (a.re * b.im - a.im * b.re) / (SQR(a.re) + SQR(a.im))};
    return r;
}

static Complex unary_minus(Complex a) {
    Complex r = {a.re * -1, a.im * -1};
    return r;
}

static Complex natural_log(Complex a) {
    Complex r = {logf(sqrtf(SQR(a.re) + SQR(a.im))), atanf(a.im / a.re)};
    return r;
}

static Complex empty_operation(Complex a) { return a; }

t_operators_table OPERATORS = {
    {"(", ")", "-", "~", "+", "*", "/", "ln"},
    {1, 1, 2, 4, 2, 3, 3, 5},
    {empty_operation, empty_operation, substraction, unary_minus, addition,
     multiplication, division, natural_log}};
t_array_of_numerals_chars NUMERALS = {'0', '1', '2', '3', '4',
                                      '5', '6', '7', '8', '9'};

int operator_num_by_str(char *opr_str) {
    int num = 0;
    while (num < NUM_OF_OP && strcmp(opr_str, OPERATORS.operator_str[num]) != 0)
        num += 1;
    if (num == NUM_OF_OP) num = -1;
    return num;
}

static int ch_is_num(char c) {
    int i = 0;
    int n = AMOUNT_OF_NUMERALS - 1;
    while (i < n && NUMERALS[i] != c) i++;
    return c == NUMERALS[i];
}

int is_const(char *s) { return ch_is_num(s[0]); }

char *complex_to_string(Complex *p_complex) {
    Complex a = *p_complex;
    static char complex_str[256] = "";
    complex_str[0] = '\0';
    a.im = fabsf(a.im);
    sprintf(complex_str, "%.2f%c%+.2f%c%c", a.re, ' ', a.im, 'i', '\0');
    return complex_str;
}

Complex perfome_binary_operation(Complex a, Complex b,
                                 Complex operation(Complex, Complex)) {
    return operation(a, b);
}

Complex perfome_unary_operation(Complex a, Complex operation(Complex)) {
    return operation(a);
}

t_operators_binary_func take_binary_operator(int operator_num) {
    return (t_operators_binary_func)OPERATORS.operator_func[operator_num];
}

t_operators_unary_func take_unary_operator(int operator_num) {
    return (t_operators_unary_func)OPERATORS.operator_func[operator_num];
}

char *get_word(char *s, int *word_begin_index) {
    static char word[1024] = "";
    int s_len = strlen(s);
    while (s[*word_begin_index] <= ' ' && *word_begin_index < s_len) {
        *word_begin_index += 1;
    }
    word[0] = '\0';
    int word_end = 0;
    while (s[*word_begin_index] > ' ' && *word_begin_index < s_len) {
        word[word_end] = s[*word_begin_index];
        *word_begin_index += 1;
        word_end += 1;
    }
    word[word_end] = '\0';
    return word;
}

int count_words(char *s) {
    int words_amount = 0;
    int word_begin = 0;
    while (strlen(get_word(s, &word_begin)) != 0) {
        words_amount += 1;
    }
    return words_amount;
}