#ifndef lcthw_stack_h
#define lcthw_stack_h

#include <lcthw/list.h>

typedef struct Stack
{
    List *list;
} Stack;

void *Stack_create()
{
    Stack *stack = calloc(1, sizeof(Stack));
    if (!stack)
        return NULL;
    stack->list = List_create();
    if (!stack->list)
    {
        free(stack);
        return NULL;
    }
    return stack;
}
void Stack_destroy(Stack *stack)
{
    if (stack)
    {
        List_destroy(stack->list);
        free(stack);
    }
}
int Stack_count(Stack *stack)
{
    return List_count(stack->list);
}

void *Stack_peek(Stack *stack)
{
    return List_last(stack->list);
}
void Stack_push(Stack *stack, void *value)
{
    List_push(stack->list, value);
}
void *Stack_pop(Stack *stack)
{
    return List_pop(stack->list);
}
#define STACK_FOREACH(stack, cur) \
    LIST_FOREACH(stack->list, first, next, cur)

#endif