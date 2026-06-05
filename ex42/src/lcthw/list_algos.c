#include <stdlib.h>
#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>

void ListNode_swap(ListNode *a, ListNode *b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}

int List_bubble_sort(List *list, List_compare cmp)
{
    int n = List_count(list);
    for (int i = 0; i < n - 1; i++)
    {
        int sorted = 1;
        ListNode *cur = list->first;
        for (int j = 0; j < n - i - 1; j++)
        {
            if (cur->next && cmp(cur->value, cur->next->value) > 0)
            {
                ListNode_swap(cur, cur->next);
                sorted = 0;
            }
            cur = cur->next;
        }
        if (sorted)
        {
            break;
        }
    }
    return 0;
}

List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();
    void *val = NULL;
    while (List_count(left) > 0 && List_count(right) > 0)
    {
        if (cmp(List_first(left), List_first(right)) <= 0)
        {
            val = List_shift(left);
        }
        else
        {
            val = List_shift(right);
        }
        List_push(result, val);
    }
    while (List_count(left) > 0)
    {
        val = List_shift(left);
        List_push(result, val);
    }
    while (List_count(right) > 0)
    {
        val = List_shift(right);
        List_push(result, val);
    }
    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if (List_count(list) <= 1)
    {
        return list;
    }
    int mid = List_count(list) / 2;
    List *left = List_create();
    List *right = List_create();
    for (ListNode *cur = list->first; cur != NULL; cur = cur->next)
    {
        if (mid > 0)
        {
            List_push(left, cur->value);
        }
        else
        {
            List_push(right, cur->value);
        }
        mid--;
    }
    List *sorted_left = List_merge_sort(left, cmp);
    List *sorted_right = List_merge_sort(right, cmp);
    List *merged = List_merge(sorted_left, sorted_right, cmp);
    if (sorted_left != left)
        List_destroy(left);
    if (sorted_right != right)
        List_destroy(right);
    List_destroy(sorted_left);
    List_destroy(sorted_right);
    return merged;
}

int List_insert_sorted(List *list, void *value, List_compare cmp)
{
    check(list != NULL, "list can't be NULL.");
    check(cmp != NULL, "cmp can't be NULL.");

    // 空链表：新节点既是 first，也是 last
    if (List_count(list) == 0)
    {
        List_push(list, value);
        return 0;
    }

    // 插入到头部
    if (cmp(value, list->first->value) <= 0)
    {
        List_unshift(list, value);
        return 0;
    }

    // 插入到中间

    ListNode *cur = list->first;

    while (cur != NULL)
    {
        if (cmp(value, cur->value) <= 0)
        {
            ListNode *node = calloc(1, sizeof(ListNode));
            check_mem(node);

            node->value = value;
            node->next = cur;
            node->prev = cur->prev;

            cur->prev->next = node;
            cur->prev = node;

            list->count++;
            return 0;
        }

        cur = cur->next;
    }

    // 插入到尾部

    List_push(list, value);
    return 0;

error:
    return -1;
}

typedef struct ListNodePair
{
    ListNode *first;
    ListNode *last;
} ListNodePair;

static ListNode *ListNode_cut(ListNode *head, int count)
{
    if (head == NULL)
    {
        return NULL;
    }

    while (--count > 0 && head->next != NULL)
    {
        head = head->next;
    }

    ListNode *second = head->next;

    head->next = NULL;

    if (second != NULL)
    {
        second->prev = NULL;
    }

    return second;
}

static void ListNode_append(ListNodePair *result, ListNode *node)
{
    node->prev = result->last;
    node->next = NULL;

    if (result->last != NULL)
    {
        result->last->next = node;
    }
    else
    {
        result->first = node;
    }

    result->last = node;
}

static ListNodePair ListNode_merge_runs(
    ListNode *left,
    ListNode *right,
    List_compare cmp)
{
    ListNodePair result = {NULL, NULL};

    while (left != NULL && right != NULL)
    {
        ListNode *take = NULL;

        if (cmp(left->value, right->value) <= 0)
        {
            take = left;
            left = left->next;
        }
        else
        {
            take = right;
            right = right->next;
        }

        ListNode_append(&result, take);
    }

    ListNode *rest = left != NULL ? left : right;

    while (rest != NULL)
    {
        ListNode *next = rest->next;

        ListNode_append(&result, rest);

        rest = next;
    }

    return result;
}

List *List_merge_sort_bottom_up(List *list, List_compare cmp)
{
    check(list != NULL, "list can't be NULL.");
    check(cmp != NULL, "cmp can't be NULL.");

    if (List_count(list) <= 1)
    {
        return list;
    }

    int width = 1;
    int count = List_count(list);

    while (width < count)
    {
        ListNode *cur = list->first;
        ListNodePair output = {NULL, NULL};

        while (cur != NULL)
        {
            ListNode *left = cur;
            ListNode *right = ListNode_cut(left, width);

            cur = ListNode_cut(right, width);

            ListNodePair merged = ListNode_merge_runs(left, right, cmp);

            if (output.first == NULL)
            {
                output.first = merged.first;
                output.last = merged.last;
            }
            else
            {
                output.last->next = merged.first;

                if (merged.first != NULL)
                {
                    merged.first->prev = output.last;
                }

                output.last = merged.last;
            }
        }

        list->first = output.first;
        list->last = output.last;

        if (list->first != NULL)
        {
            list->first->prev = NULL;
        }

        if (list->last != NULL)
        {
            list->last->next = NULL;
        }

        width *= 2;
    }

    return list;

error:
    return NULL;
}