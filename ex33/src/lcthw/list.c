#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <assert.h>

static int List_invariant(List *list)
{
    if (list == NULL)
        return 0;

    if (list->count < 0)
        return 0;

    if (list->count == 0)
    {
        if (list->first != NULL)
            return 0;
        if (list->last != NULL)
            return 0;
        return 1;
    }

    if (list->first == NULL)
        return 0;
    if (list->last == NULL)
        return 0;

    if (list->first->prev != NULL)
        return 0;
    if (list->last->next != NULL)
        return 0;

    int count = 0;
    ListNode *cur = list->first;
    ListNode *prev = NULL;

    while (cur != NULL)
    {
        if (cur->prev != prev)
            return 0;

        prev = cur;
        cur = cur->next;
        count++;
        // 避免链表内部有环
        if (count > list->count)
            return 0;
    }

    if (prev != list->last)
        return 0;

    if (count != list->count)
        return 0;

    return 1;
}

List *List_create()
{
    return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
    assert(list != NULL);
    List_invariant(list);
    LIST_FOREACH(list, first, next, cur)
    {
        if (cur->prev)
        {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List *list)
{
    assert(list != NULL);
    List_invariant(list);

    LIST_FOREACH(list, first, next, cur)
    {
        free(cur->value);
    }
}

void List_clear_destroy(List *list)
{
    assert(list != NULL);
    List_invariant(list);

    ListNode *cur = list->first;
    ListNode *next = NULL;
    while (cur)
    {
        next = cur->next;
        free(cur->value);
        free(cur);
        cur = next;
    }
    free(list);
}

void List_push(List *list, void *value)
{
    // pointer list should be pointing to struct List, not NULL
    assert(list != NULL);
    ListNode *node = calloc(1, sizeof(ListNode));
    // 检查内存是否申请成功
    check_mem(node);

    node->value = value;

    if (list->last == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;
    List_invariant(list);

error:
    return;
}

void *List_pop(List *list)
{
    assert(list != NULL);
    List_invariant(list);

    ListNode *node = list->last;
    List_invariant(list);

    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_shift(List *list)
{
    assert(list != NULL);
    List_invariant(list);

    ListNode *node = list->first;
    List_invariant(list);

    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
    assert(list != NULL);
    List_invariant(list);

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if (list->first == NULL)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;
    List_invariant(list);

error:
    return;
}

void *List_remove(List *list, ListNode *node)
{
    assert(list != NULL);
    List_invariant(list);

    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if (node == list->first && node == list->last)
    {
        list->first = NULL;
        list->last = NULL;
    }
    else if (node == list->first)
    {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    }
    else if (node == list->last)
    {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    }
    else
    {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    result = node->value;
    free(node);
    List_invariant(list);

error:
    return result;
}

void *List_copy(List *list, char mode)
{
    assert(list != NULL);
    List_invariant(list);

    int count = 0;

    List *new_list = List_create();
    List_invariant(new_list);

    LIST_FOREACH(list, first, next, cur)
    {
        // data_copy
        if (mode == 'd')
        {
            void *new_value = strdup((const char *)cur->value);
            List_push(new_list, new_value);
        }
        else
        {
            void *new_value = cur->value;
            List_push(new_list, new_value);
        }
        // push data into new link list
        count++;
    }
    List_invariant(new_list);
    return new_list;
}

void *List_concat(List *list1, List *list2)
{
    assert(list1 != NULL);
    assert(list2 != NULL);
    List_invariant(list1);
    List_invariant(list2);

    List *new_list = List_create();
    List_invariant(new_list);

    LIST_FOREACH(list1, first, next, cur)
    {
        List_push(new_list, cur->value);
    }

    LIST_FOREACH(list2, first, next, cur)
    {
        List_push(new_list, cur->value);
    }

    List_invariant(new_list);
    assert(new_list->count == list1->count + list2->count);
    return new_list;
}

ListPair *List_.vide(List *list, int position)
{
    // position >=1 and < count
    assert(list != NULL);
    List_invariant(list);
    assert(position >= 1 && position < list->count);

    ListPair *pair = malloc(sizeof(ListPair));
    pair->first = NULL;
    pair->second = NULL;

    if (list->first == list->last)
    {
        printf("only one node detected not dividable.");
        return pair;
    }
    List *p_list = List_create();
    List *n_list = List_create();
    ListNode *cur = list->first;
    for (int i = 0; i < list->count; i++)
    {
        void *new_value = strdup((const char *)cur->value);
        if (i < position)
        {
            List_push(p_list, new_value);
        }
        else
        {
            List_push(n_list, new_value);
        }
        cur = cur->next;
    }
    pair->first = p_list;
    pair->second = n_list;
    List_invariant(pair->first);
    List_invariant(pair->second);
    return pair;
}