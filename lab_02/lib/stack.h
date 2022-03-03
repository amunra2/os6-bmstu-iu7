#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>

#define STACK_SIZE 1000000
#define MAX_NAME 100


struct stackElement
{
    char fileName[MAX_NAME];
    int depth;
};


struct stack
{
    struct stackElement elements[STACK_SIZE];
    int topInd; 
};


void initStack(struct stack *stk);
int isEmpty(struct stack *stk);
int isOverflow(struct stack *stk);
void pushStack(struct stack *stk, struct stackElement *elem);
struct stackElement popStack(struct stack *stk);

#endif
