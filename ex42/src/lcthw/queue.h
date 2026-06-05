#ifndef lcthw_queue_h
#define lcthw_queue_h

#include <lcthw/list.h>

typedef struct Queue
{
    List *list;
} Queue;

void *Queue_create()
{
    Queue *queue = calloc(1, sizeof(Queue));
    if (!queue)
        return NULL;
    queue->list = List_create();
    if (!queue->list)
    {
        free(queue);
        return NULL;
    }
    return queue;
}

void Queue_destroy(Queue *queue)
{
    if (queue)
    {
        List_destroy(queue->list);
        free(queue);
    }
}

int Queue_count(Queue *queue)
{
    return List_count(queue->list);
}

void *Queue_peek(Queue *queue)
{
    return List_last(queue->list);
}

void Queue_send(Queue *queue, void *value)
{
    List_unshift(queue->list, value);
}
void *Queue_recv(Queue *queue)
{
    return List_pop(queue->list);
}
#define QUEUE_FOREACH(queue, cur) \
    LIST_FOREACH(queue->list, first, next, cur)

#endif