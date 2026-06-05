#include <lcthw/darray.h>
#include <lcthw/dbg.h>
#include <assert.h>

DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = calloc(1, sizeof(DArray));
    array->element_size = element_size;
    array->max = initial_max;
    array->end = 0;
    array->expand_rate = DEFAULT_EXPAND_RATE;
    array->contents = calloc(initial_max, sizeof(void *));
    return array;
}

void DArray_destroy(DArray *array)
{
    if (array)
    {
        if (array->contents)
        {
            free(array->contents);
        }
        free(array);
    }
}

void DArray_clear(DArray *array)
{
    if (array)
    {
        for (int i = 0; i < array->max; i++)
        {
            if (array->contents[i] != NULL)
            {
                free(array->contents[i]);
                array->contents[i] = NULL;
            }
        }
    }
}

void DArray_expand(DArray *array)
{
    void **contents = NULL;
    // 原地扩容
    contents = realloc(array->contents, 2 * array->max * sizeof(void *));
    array->contents = contents;
    array->max *= 2;
}

void DArray_contract(DArray *array)
{
    int new_size = array->max;
    while (new_size > 4 * array->end && new_size / 2 > 0)
    {
        new_size /= 2;
    }
    
    void **contents = NULL;
    // 原地缩容
    contents = realloc(array->contents, new_size * sizeof(void *));
    array->contents = contents;
    array->max = new_size;
}

void DArray_push(DArray *array, void *el)
{
    DArray_set(array, array->end, el);
    array->end++;
    if (DArray_end(array) >= DArray_max(array))
    {
        DArray_expand(array);
    }
}

void *DArray_pop(DArray *array)
{
    void *value = DArray_remove(array, array->end - 1);
    array->end--;
    if (DArray_end(array) < DArray_max(array) / 4)
    {
        DArray_contract(array);
    }
    return value;

}

void DArray_clear_destroy(DArray *array)
{
    if (array)
    {
        DArray_clear(array);
        DArray_destroy(array);
    }
}