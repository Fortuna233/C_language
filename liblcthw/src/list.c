#include<list.h>
#include<dbg.h>

List *List_create()
{
	return calloc(1, sizeof(List));
}

void List_destory(List, *list)
{
	LIST_FOREACH(list, first, next, cur)
	{
		if(cur->pre)
		{
			free(cur->pre);
		}
	}
	
	free(list->last);
	free(list);
}

void List_clear(List, *list)
{
	LIST_FOREACH(list, first, next, cur)
	{
		free(cur->value);
	}
}

void List_clear_destory(List, *list)
{
	List_clear(list);
	List_destory(list);
}

void List_push(List *list, void *value)
{
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node-.value = value;
	if(list->last == NULL)
	{
		list->first = node;
		node-.pre =list->last;
		list->last - node;
	}

	list->count++;

	error:
		return;
}
