#ifndef BIGNUM_H
#define BIGNUM_H

typedef struct Link {
    int digit;
    struct Link* prev;
    struct Link* next;
} Link;

typedef struct {
    Link* head;
    Link* tail;
} BigNum;

/* Constructors and destructors */
BigNum* bn_from_string(const char* s);
char* bn_to_string(const BigNum* a);
void bn_free(BigNum* a);

/* Operations */
int bn_compare(const BigNum* A, const BigNum* B);
BigNum* bn_add(const BigNum* A, const BigNum* B);
BigNum* bn_sub(const BigNum* A, const BigNum* B);   /* returns 0 if A < B */
BigNum* bn_mul(const BigNum* A, const BigNum* B);

#endif
