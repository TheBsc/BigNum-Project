#include <stdio.h>
#include <stdlib.h>
#include "bignum.h"

int main(void) {
    BigNum* a = bn_from_string("987654321");
    BigNum* b = bn_from_string("123456789");

    BigNum* sum = bn_add(a, b);
    BigNum* diff = bn_sub(a, b);
    BigNum* prod = bn_mul(a, b);

    char* s_sum = bn_to_string(sum);
    char* s_diff = bn_to_string(diff);
    char* s_prod = bn_to_string(prod);

    printf("A: 987654321\nB: 123456789\n");
    printf("Compare(A,B) = %d\n", bn_compare(a, b));
    printf("Sum  = %s\n", s_sum);
    printf("Diff = %s\n", s_diff);
    printf("Prod = %s\n", s_prod);


    BigNum* c = bn_from_string("123");
    BigNum* d = bn_from_string("999");
    BigNum* diff2 = bn_sub(c, d);
    char* s_diff2 = bn_to_string(diff2);
    printf("123 - 999 = %s (A<B)\n", s_diff2);

    // free memory
    return 0;
}
