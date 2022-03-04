#include "stack.h"


void initStack(struct stack *stk)
{
    stk->topInd = 0;
}


int isEmpty(struct stack *stk)
{
    return (stk->topInd == 0) ? 1 : 0;
}


int isOverflow(struct stack *stk)
{
    return (stk->topInd >= STACK_SIZE) ? 1 : 0;
}


void push(struct stack *stk, struct stackElement *elem)
{
    if (isOverflow(stk))   
    {
        printf("\nError: Stack if overflowed\n");
        return;
    }

    strcpy(stk->elements[stk->topInd].fileName, elem->fileName);
    stk->elements[stk->topInd].depth = elem->depth;

    stk->topInd++;
}


struct stackElement pop(struct stack *stk)
{
    struct stackElement elem = {.fileName = "", .depth = 0};

    if (isEmpty(stk))
    {
        printf("\nError: Stack is empty\n");
        return elem;
    }

    stk->topInd--;

    strcpy(elem.fileName, stk->elements[stk->topInd].fileName);
    elem.depth = stk->elements[stk->topInd].depth;


    return elem;
}
