#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

typedef struct ListNode
{
    struct ListNode *next;
    struct ListNode *prev;
    // 不知道数据类型，需要强制转换
    void *value;
} ListNode;

typedef struct List
{
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

// void *: return generic pointer
void List_push(List *list, void *value);
void *List_pop(List *list);

void List_unshift(List *list, void *value);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);

void *List_copy(List *list, char mode);
void *List_concat(List *list1, List *list2);
void *List_divide(List *list);

#define List_shallow_copy(list) List_copy(list, NULL)
// L链表指针
// S first或者last
// M 移动方向
// 当前节点
#define LIST_FOREACH(L, S, M, V) \
    for(ListNode *_node = (L)->S, *V = _node; _node != NULL; V = _node = _node->M)

#endif
