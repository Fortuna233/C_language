#include "minunit.h"
#include <lcthw/darray.h>

static DArray *array = NULL;
static int *val1 = NULL;
static int *val2 = NULL;

static int expected_contract_max(int max, int end)
{
    int new_size = max;

    while(new_size > 4 * end && new_size / 2 > 0) {
        new_size /= 2;
    }

    return new_size;
}

static int expected_push_max(int max, int end, int pushes)
{
    int i = 0;

    for(i = 0; i < pushes; i++) {
        end++;
        if(end >= max) {
            max *= 2;
        }
    }

    return max;
}

char *test_create()
{
    array = DArray_create(sizeof(int), 100);
    mu_assert(array != NULL, "DArray_create failed.");
    mu_assert(array->contents != NULL, "contents are wrong in darray");
    mu_assert(array->end == 0, "end isn't at the right spot");
    mu_assert(array->element_size == sizeof(int), "element size is wrong.");
    mu_assert(array->max == 100, "wrong max length on initial size");

    return NULL;
}

char *test_destroy()
{
    DArray_destroy(array);

    return NULL;
}

char *test_new()
{
    val1 = DArray_new(array);
    mu_assert(val1 != NULL, "failed to make a new element");

    val2 = DArray_new(array);
    mu_assert(val2 != NULL, "failed to make a new element");

    return NULL;
}

char *test_set()
{
    DArray_set(array, 0, val1);
    DArray_set(array, 1, val2);

    return NULL;
}

char *test_get()
{
    mu_assert(DArray_get(array, 0) == val1, "Wrong first value.");
    mu_assert(DArray_get(array, 1) == val2, "Wrong second value.");

    return NULL;
}

char *test_remove()
{
    int *val_check = DArray_remove(array, 0);
    mu_assert(val_check != NULL, "Should not get NULL.");
    mu_assert(*val_check == *val1, "Should get the first value.");
    mu_assert(DArray_get(array, 0) == NULL, "Should be gone.");
    DArray_free(val_check);

    val_check = DArray_remove(array, 1);
    mu_assert(val_check != NULL, "Should not get NULL.");
    mu_assert(*val_check == *val2, "Should get the second value.");
    mu_assert(DArray_get(array, 1) == NULL, "Should be gone.");
    DArray_free(val_check);

    return NULL;
}

char *test_expand_contract()
{
    int old_max = array->max;
    int expected_max = 0;

    DArray_expand(array);
    mu_assert(array->max == old_max * 2, "Wrong size after expand.");

    expected_max = expected_contract_max(array->max, array->end);
    DArray_contract(array);
    mu_assert(array->max == expected_max, "Wrong size after contract.");

    expected_max = expected_contract_max(array->max, array->end);
    DArray_contract(array);
    mu_assert(array->max == expected_max, "Wrong size after second contract.");

    return NULL;
}

char *test_push_pop()
{
    int i = 0;
    int start_end = array->end;
    int expected_max = expected_push_max(array->max, array->end, 1000);

    for(i = 0; i < 1000; i++) {
        int *val = DArray_new(array);
        *val = i * 333;
        DArray_push(array, val);
    }

    mu_assert(array->end == start_end + 1000, "Wrong end after push.");
    mu_assert(array->max == expected_max, "Wrong max size after push.");

    for(i = 999; i >= 0; i--) {
        int *val = DArray_pop(array);
        mu_assert(val != NULL, "Shouldn't get a NULL.");
        mu_assert(*val == i * 333, "Wrong value.");
        DArray_free(val);
    }

    mu_assert(array->end == start_end, "Wrong end after pop.");
    mu_assert(array->max >= array->end, "Array max should not be smaller than end.");

    return NULL;
}


char * all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_new);
    mu_run_test(test_set);
    mu_run_test(test_get);
    mu_run_test(test_remove);
    mu_run_test(test_expand_contract);
    mu_run_test(test_push_pop);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);