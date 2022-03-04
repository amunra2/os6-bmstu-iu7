#include "stack.h"


// Стек -- список
int isEmpty(node_t **stack)
{
    return (!(*stack)) ? 1 : 0;
}



node_t *create_node(char *fileName, int depth)
{
    node_t *tmp_elem = malloc(sizeof(node_t));

    if (!tmp_elem)
    {
        return NULL;
    }

    strcpy(tmp_elem->fileName, fileName);
    tmp_elem->depth = depth;

    tmp_elem->next = NULL;

    return tmp_elem;
}



int push(node_t **stack, node_t *elem)
{
    node_t *tmp = create_node(elem->fileName, elem->depth);

    if (!tmp)
    {
        return -1;
    }

    if (!(*stack))
    {
        *stack = tmp;

        return 0;
    }

    node_t *tmp_head = *stack;

    while (tmp_head->next != NULL)
    {
        tmp_head = tmp_head->next;
    }

    tmp_head->next = tmp;

    return 0;
}


node_t *pop(node_t **stack)
{
    if (!stack || !(*stack))
    {
        return NULL;
    }

    if ((*stack)->next == NULL) // если 1 элемент в списке
    {
        node_t *tmp_node = create_node((*stack)->fileName, (*stack)->depth);

        free(*stack);
        *stack = NULL;

        return tmp_node;
    }

    node_t *tmp_head = *stack;

    while (tmp_head->next->next != NULL)
    {
        tmp_head = tmp_head->next;
    }

    node_t *tmp_node = create_node(tmp_head->next->fileName, tmp_head->next->depth);

    free(tmp_head->next);
    tmp_head->next = NULL;

    return tmp_node;
}




//* Стек -- массив
// void initStack(struct stack *stk)
// {
//     stk->topInd = 0;
// }


// int isEmpty(struct stack *stk)
// {
//     return (stk->topInd == 0) ? 1 : 0;
// }


// int isOverflow(struct stack *stk)
// {
//     return (stk->topInd >= STACK_SIZE) ? 1 : 0;
// }


// void push(struct stack *stk, struct stackElement *elem)
// {
//     if (isOverflow(stk))   
//     {
//         printf("\nError: Stack if overflowed\n");
//         return;
//     }

//     strcpy(stk->elements[stk->topInd].fileName, elem->fileName);
//     stk->elements[stk->topInd].depth = elem->depth;

//     stk->topInd++;
// }


// struct stackElement pop(struct stack *stk)
// {
//     struct stackElement elem = {.fileName = "", .depth = 0};

//     if (isEmpty(stk))
//     {
//         printf("\nError: Stack is empty\n");
//         return elem;
//     }

//     stk->topInd--;

//     strcpy(elem.fileName, stk->elements[stk->topInd].fileName);
//     elem.depth = stk->elements[stk->topInd].depth;


//     return elem;
// }
