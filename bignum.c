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
/* multiply one digit with shift */
static BigNum* bn_mul_single(const BigNum* A, int d, int shift) {
    // 0 multiplied by anything is 0
    if (d == 0) {
        BigNum* result = malloc(sizeof(BigNum));
        result->head = node_new(0);
        result->tail = result->head;
        return result;
    }

    BigNum* result = malloc(sizeof(BigNum));
    result->head = NULL;
    result->tail = NULL;
    
    Link* currA = A->tail;
    int carry = 0;

    // 1. Perform the multiplication (e.g., 123 * 4 = 492)
    //    We iterate from the tail, just like in bn_add.
    while (currA != NULL || carry != 0) {
        int digitA = (currA != NULL) ? currA->digit : 0;
        
        int product = (digitA * d) + carry;
        int newDigit = product % 10;
        carry = product / 10;

        Link* newNode = node_new(newDigit);
        
        // Add to head (builds the result in reverse: 2, then 9, then 4)
        if (result->head == NULL) {
            result->head = newNode;
            result->tail = newNode;
        } else {
            newNode->next = result->head;
            result->head->prev = newNode;
            result->head = newNode;
        }
        
        if (currA != NULL) currA = currA->prev;
    }

    // 2. Add 'shift' zeros to the tail (e.g., for 49200)
    for (int i = 0; i < shift; i++) {
        Link* zeroNode = node_new(0);
        
        // We must check if the list is [0]
        if (result->head->digit == 0 && result->head->next == NULL) {
             // Do nothing if the result is already 0
        } else {
            // Add the zero to the end of the list
            result->tail->next = zeroNode;
            zeroNode->prev = result->tail;
            result->tail = zeroNode;
        }
    }
    
    return result;
}

/* multiplication */
/* multiplication */
BigNum* bn_mul(const BigNum* A, const BigNum* B) {
    
    // Create a BigNum to store the final sum, initialized to 0
    BigNum* totalSum = malloc(sizeof(BigNum));
    totalSum->head = node_new(0);
    totalSum->tail = totalSum->head;

    Link* currB = B->tail;
    int shift = 0;

    // Iterate through B's digits from right-to-left
    while (currB != NULL) {
        int digitB = currB->digit;
        
        // 1. Get the partial product for this digit
        //    (e.g., A * 5, then A * 40, then A * 300, etc.)
        BigNum* partialProduct = bn_mul_single(A, digitB, shift);
        
        // 2. Add the partial product to the total sum
        BigNum* tempSum = bn_add(totalSum, partialProduct);
        
        // 3. Free the OLD totalSum and the partialProduct
        //    This is critical for memory management [cite: 69]
        bn_free(totalSum);
        bn_free(partialProduct);
        
        // 4. The new totalSum is the one we just calculated
        totalSum = tempSum;
        
        // 5. Increment shift for the next digit
        shift++;
        currB = currB->prev;
    }
    
    return totalSum;
}


void bn_free(BigNum* a) {
    if (a == NULL) {
        return;
    }

    Link* current = a->head;
    while (current != NULL) {
        Link* temp = current; // Get the node to free
        current = current->next; // Move to the next node
        free(temp); // Free the old node
    }
    
    // Finally, free the BigNum struct itself
    free(a);
}