#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>

#define STACK_SIZE 1000000
#define MAX_NAME 200


struct stackElement
{
    char fileName[MAX_NAME];
    long depth;
};


struct stack
{
    struct stackElement elements[STACK_SIZE];
    int topInd; 
};


void initStack(struct stack *stk);
int isEmpty(struct stack *stk);
int isOverflow(struct stack *stk);
void push(struct stack *stk, struct stackElement *elem);
struct stackElement pop(struct stack *stk);

#endif
