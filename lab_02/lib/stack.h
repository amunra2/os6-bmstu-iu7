#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STACK_SIZE 1000000
#define MAX_NAME 200



// Стек -- на списке
typedef struct node_s
{
    char fileName[MAX_NAME];
    long depth;

    struct node_s *next;
} node_t;


int isEmpty(node_t **stack);
node_t *create_node(char *fileName, int depth);
int push(node_t **stack, node_t *elem);
node_t *pop(node_t **stack);



//* Стек -- на массиве
// struct stackElement
// {
//     char fileName[MAX_NAME];
//     long depth;
// };


// struct stack
// {
//     struct stackElement elements[STACK_SIZE];
//     int topInd; 
// };


// void initStack(struct stack *stk);
// int isEmpty(struct stack *stk);
// int isOverflow(struct stack *stk);
// void push(struct stack *stk, struct stackElement *elem);
// struct stackElement pop(struct stack *stk);

#endif
