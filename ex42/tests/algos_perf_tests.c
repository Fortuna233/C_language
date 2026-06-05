
#include "minunit.h"
#include <lcthw/list.h>
#include <lcthw/list_algos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

double time_now(void)
{
    return (double)clock() / CLOCKS_PER_SEC;
}

int int_cmp(const void *a, const void *b)
{
    int av = *(const int *)a;
    int bv = *(const int *)b;

    if (av < bv)
        return -1;
    if (av > bv)
        return 1;
    return 0;
}

List *create_random_int_list(int count)
{
    List *list = List_create();
    check(list != NULL, "failed to create list.");

    for (int i = 0; i < count; i++)
    {
        int *value = malloc(sizeof(int));
        check_mem(value);

        *value = rand();
        List_push(list, value);
    }

    return list;

error:
    return NULL;
}

int is_sorted(List *list)
{
    if (List_count(list) <= 1)
    {
        return 1;
    }

    LIST_FOREACH(list, first, next, cur)
    {
        if (cur->next != NULL)
        {
            if (int_cmp(cur->value, cur->next->value) > 0)
            {
                return 0;
            }
        }
    }

    return 1;
}

char *test_random_list_sort_performance()
{
    int num_size[] = {1024, 2048, 4096, 8192};
    int times = sizeof(num_size) / sizeof(num_size[0]);
    double bubble_times[times];
    double merge_times[times];
    for (int i = 0; i < times; i++)
    {
        List *list1 = create_random_int_list(num_size[i]);
        double bubble_start = time_now();
        int rc = List_bubble_sort(list1, int_cmp);
        double bubble_end = time_now();
        List_clear_destroy(list1);        

        List *list2 = create_random_int_list(num_size[i]);
        double merge_start = time_now();
        list2 = List_merge_sort(list2, int_cmp);
        double merge_end = time_now();
        bubble_times[i] = bubble_end - bubble_start;
        merge_times[i] = merge_end - merge_start;
        List_clear_destroy(list2);
    }
    FILE *out = fopen("sort_perf.csv", "w");
    mu_assert(out != NULL, "failed to open csv.");
    fprintf(out, "algorithm,n,seconds\n");
    for (int i = 0; i < times; i++){
        fprintf(out, "bubble,%d,%f\n", num_size[i], bubble_times[i]);
        fprintf(out, "merge,%d,%f\n", num_size[i], merge_times[i]);
    }
    fclose(out);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_random_list_sort_performance);


    return NULL;
}

RUN_TESTS(all_tests);