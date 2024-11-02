/*
file: stack.h
author: Adam Vesely
Header file for the stack.
*/

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "common/token.h"

typedef struct stack_element {
    struct stack_element* next;
    Token* token;
} stack_elem;

typedef struct {
    stack_elem* top;
} stack_t;

// initialises the stack
void stackInit();
// pushes a new element onto the top of the stack
void stackPush(stack_t* stack, Token* token);
// pops the top element off the stack
void stackPop(stack_t* stack);
// checks if the stack is empty
bool stackIsEmpty(stack_t* stack);
// clears the entire stack and frees it
void stackClear(stack_t* stack);
// gets the top element of the stack without changing the stack
Token* stackGetTop(stack_t* stack);

#endif // STACK_H