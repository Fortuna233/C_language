#ifndef lcthw_stack_darray_h
#define lcthw_stack_darray_h

#include <lcthw/darray.h>

typedef struct Stack
{
    DArray *array;
} Stack;

void *Stack_create()
{
    Stack *stack = calloc(1, sizeof(Stack));
    if (!stack)
        return NULL;
    stack->array = DArray_create(sizeof(void *), 4);
    if (!stack->array)
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
        DArray_destroy(stack->array);
        free(stack);
    }
}
int Stack_count(Stack *stack)
{
    return DArray_end(stack->array);
}

void *Stack_peek(Stack *stack)
{
    return DArray_last(stack->array);
}
void Stack_push(Stack *stack, void *value)
{
    DArray_push(stack->array, value);
}
void *Stack_pop(Stack *stack)
{
    return DArray_pop(stack->array);
}
#define STACK_FOREACH(stack, index) \
    for (int i = index; i < DArray_end(stack->array); i++)

#endif