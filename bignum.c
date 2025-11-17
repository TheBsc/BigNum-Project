#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bignum.h"

/* utility */
static Link* node_new(int d) {
    Link* n = malloc(sizeof(Link));
    n->digit = d;
    n->prev = n->next = NULL;
    return n;
}


/* compare */
int bn_compare(const BigNum* A, const BigNum* B) {
    int lenA = 0;
    int lenB = 0;

    Link* currentA;
    Link* currentB;

    // Calcualte the len of A
    for (currentA = A->head; currentA != NULL; currentA = currentA->next) {
        lenA++;
    }

    // Calculate the len of B
    for (currentB = B->head; currentB != NULL; currentB = currentB->next) {
        lenB++;
    }

    if (lenA < lenB) {
        return -1;
    } 

    if (lenA > lenB) {
        return 1;
    }

    currentA = A->head;
    currentB = B->head;

    while (currentA != NULL) {
        if (currentA->digit < currentB->digit) {
            return -1;
        }
        if (currentA->digit > currentB->digit) {
            return 1;
        }

        // Digits are the same
        currentA = currentA->next;
        currentB = currentB->next;
    }

    // Loop finished both are equal , return 0
    return 0;
}

/* addition */
BigNum* bn_add(const BigNum* A, const BigNum* B) {
    // Create an empty bignum to store the result
    BigNum* result = malloc(sizeof(BigNum));
    result->head = NULL;
    result->tail = NULL;

    // Set up pointers. 
    // Because we might have carry- We start at the tail
    Link* currentA = A->tail;
    Link* currentB = B->tail;

    int carry = 0;
    while (currentA != NULL || currentB != NULL || carry != 0) {

        // Get the current digit, if its not null, if it is , pick 0
        int digitA = (currentA != NULL) ? currentA->digit : 0;
        int digitB = (currentB != NULL) ? currentB->digit : 0;

        // Sum
        int sum = digitA + digitB + carry;

        // Handle carry
        int newDigit = sum % 10;
        carry = sum / 10;

        // Create a new node for the result digit
        Link* newNode = node_new(newDigit);

        // Add the new node to the **HEAD** of the result list
        if (result->head == NULL) {
            // This means the list is empty
            result->head = newNode;
            result->tail = newNode;
        } else {
            // If the list isn't empty
            // Point the new node next to the current head of the list 
            // Examples: If use is [9] and the newNode is [7]
            // this makes newNode [7] point forward to [9]
            newNode->next = result->head;
            //  Point the old head's 'prev' (previous) back to the new node.
            //    Example: This makes the old head [9] point backward to the
            //    new node [7], completing the double link: [7] <-> [9].
            result->head->prev = newNode;

            //  Update the 'result' list's official head pointer to be our new node.
            //    Example: The list's head is no longer [9], it is now [7].
            //    The list is now [7] <-> [9].
            result->head = newNode;
        }

        if (currentA != NULL) {
            currentA = currentA->prev;
        }

        if (currentB != NULL) {
            currentB = currentB->prev;
        }
    }

    return result;
}

/* subtraction: returns 0 if A < B */
BigNum* bn_sub(const BigNum* A, const BigNum* B) {
    //create an empty bignum to store the result
    BigNum* result = malloc(sizeof(BigNum));
    result->head = NULL;
    result->tail = NULL;

    // set up pointers. 
    // Because we might have borrow- We start at the tail
    Link* currentA = A->tail;
    Link* currentB = B->tail;

    int borrow = 0; // no borrow at the begining

    // Loop while there are digits in A
    // (we assume A >= B, so we don't need to check B's digits)
    // Also, we don't need to check for borrow at the end,
    // because if A >= B, we will never have a leftover borrow


    while (currentA != NULL) {

        //subtract borrow from currentA digit
        int digitA = currentA->digit - borrow;
        
        // Get the current digit of B, or 0 if B is exhausted
        int digitB = (currentB != NULL) ? currentB->digit : 0;

        // reset
        borrow = 0;

        // Check if we need to borrow
        if (digitA < digitB) {
            digitA = digitA + 10; 
            borrow = 1;       
        }

        // subtraction
        int newDigit = digitA - digitB;

        // Create a new node for the result digit
        Link* newNode = node_new(newDigit);

        // Add the new node to the **HEAD** of the result list
        // Because we are processing digits from least significant to most significant,
        if (result->head == NULL) {
            // empty list
            result->head = newNode;
            result->tail = newNode;
        } else {
            // connect the new node to the current head of the list
            newNode->next = result->head;
            // update the current head's prev to point back to the new node
            result->head->prev = newNode;
            // update the head pointer of the result list to the new node
            result->head = newNode;
        }

        
        if (currentA != NULL) {
            currentA = currentA->prev;
        }

        if (currentB != NULL) {
            currentB = currentB->prev;
        }
    }

    // Remove leading zeros from the result 
    // (but leave at least one digit if the result is zero)
    // For example, if the result is 000123, we want to keep 123.
    while (result->head != NULL && result->head->digit == 0 && result->head->next != NULL) {
        Link* toFree = result->head;      // define a pointer to the leading zero node
        result->head = result->head->next;  // move the head pointer to the next node
        result->head->prev = NULL;          // set the new head's prev to NULL
        free(toFree);                     // free the old leading zero node
    }

    return result;
}

/* multiply one digit with shift */
static BigNum* bn_mul_single(const BigNum* A, int d, int shift) {

}

/* multiplication */
BigNum* bn_mul(const BigNum* A, const BigNum* B) {

}
