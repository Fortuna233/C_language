#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}

char *test_destroy()
{
    List_clear_destroy(list);

    return NULL;
}

char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/shift
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}

char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_copy()
{
    // ==============================================
    // 测试1：空链表拷贝
    // ==============================================
    List *empty_list = List_create();
    mu_assert(empty_list != NULL, "Failed to create empty list.");

    List *empty_shallow = List_copy(empty_list, 's');
    mu_assert(empty_shallow != NULL, "Failed to shallow copy empty list.");
    mu_assert(List_count(empty_shallow) == 0, "Empty shallow copy should have 0 elements.");
    List_destroy(empty_shallow);

    List *empty_deep = List_copy(empty_list, 'd');
    mu_assert(empty_deep != NULL, "Failed to deep copy empty list.");
    mu_assert(List_count(empty_deep) == 0, "Empty deep copy should have 0 elements.");
    List_destroy(empty_deep);

    List_destroy(empty_list);

    // ==============================================
    // 测试2：浅拷贝模式
    // ==============================================
    char *str1 = malloc(32);
    char *str2 = malloc(32);
    char *str3 = malloc(32);
    mu_assert(str1 != NULL && str2 != NULL && str3 != NULL, "Failed to allocate test strings.");
    strcpy(str1, "original_value_1");
    strcpy(str2, "original_value_2");
    strcpy(str3, "original_value_3");

    List *orig_list = List_create();
    mu_assert(orig_list != NULL, "Failed to create original test list.");
    List_push(orig_list, str1);
    List_push(orig_list, str2);
    List_push(orig_list, str3);
    mu_assert(List_count(orig_list) == 3, "Original list should have 3 elements.");

    List *shallow_copy = List_copy(orig_list, 's');
    mu_assert(shallow_copy != NULL, "Shallow copy returned NULL.");
    mu_assert(List_count(shallow_copy) == 3, "Shallow copy should have 3 elements.");

    char *shallow_first = (char *)List_first(shallow_copy);
    mu_assert(shallow_first != NULL, "Shallow copy first element is NULL.");
    mu_assert(strcmp(shallow_first, "original_value_1") == 0,
              "Shallow copy initial content mismatch.");

    // 修改原始数据，验证浅拷贝同步变化
    strcpy(str1, "MODIFIED_BY_ORIGINAL");
    mu_assert(strcmp(shallow_first, "MODIFIED_BY_ORIGINAL") == 0,
              "Shallow copy should reflect changes to original data.");

    List_destroy(shallow_copy);

    // ==============================================
    // 测试3：深拷贝模式
    // ==============================================
    List *deep_copy = List_copy(orig_list, 'd');
    mu_assert(deep_copy != NULL, "Deep copy returned NULL.");
    mu_assert(List_count(deep_copy) == 3, "Deep copy should have 3 elements.");

    char *deep_first = (char *)List_first(deep_copy);
    mu_assert(deep_first != NULL, "Deep copy first element is NULL.");
    mu_assert(strcmp(deep_first, "MODIFIED_BY_ORIGINAL") == 0,
              "Deep copy initial content mismatch.");

    // 修改原始数据，验证深拷贝不变
    strcpy(str1, "changed_again");
    mu_assert(strcmp(deep_first, "changed_again") != 0,
              "Deep copy should NOT reflect changes to original data.");
    mu_assert(strcmp(deep_first, "MODIFIED_BY_ORIGINAL") == 0,
              "Deep copy content should remain unchanged.");

    // 销毁原始链表，验证深拷贝仍然可用
    List_clear_destroy(orig_list);
    mu_assert(List_count(deep_copy) == 3, "Deep copy should survive original list destruction.");
    mu_assert(strcmp(deep_first, "MODIFIED_BY_ORIGINAL") == 0,
              "Deep copy data should remain valid after original list is destroyed.");

    // 验证深拷贝链表可以正常操作
    char *popped_val = List_pop(deep_copy);
    mu_assert(popped_val != NULL, "List_pop on deep copy returned NULL.");
    mu_assert(strcmp(popped_val, "original_value_3") == 0, "Deep copy pop operation failed.");
    mu_assert(List_count(deep_copy) == 2, "Deep copy count wrong after pop.");

    List_clear_destroy(deep_copy);
    return NULL;
}

char *test_concat()
{
    char *str1 = malloc(32);
    char *str2 = malloc(32);
    char *str3 = malloc(32);
    char *str4 = malloc(32);
    char *str5 = malloc(32);
    char *str6 = malloc(32);
    mu_assert(str1 != NULL && str2 != NULL && str3 != NULL, "Failed to allocate test strings.");
    strcpy(str1, "original_value_1");
    strcpy(str2, "original_value_2");
    strcpy(str3, "original_value_3");
    strcpy(str4, "original_value_4");
    strcpy(str5, "original_value_5");
    strcpy(str6, "original_value_6");

    List *list1 = List_create();
    List *list2 = List_create();
    List_push(list1, str1);
    List_push(list1, str2);
    List_push(list1, str3);

    List_push(list2, str4);
    List_push(list2, str5);
    List_push(list2, str6);
    mu_assert(list1 != NULL, "Failed to create list1.");
    mu_assert(list2 != NULL, "Failed to create list2.");
    mu_assert(List_count(list1) == 3, "list1 should have 3 elements.");
    mu_assert(List_count(list2) == 3, "list2 should have 3 elements.");

    List *new_list = List_concat(list1, list2);
    printf("new_list->count: %d\n", new_list->count);
    mu_assert(List_count(new_list) == 6, "new_list should have 6 elements.");
    mu_assert(new_list->first->value == list1->first->value, "1st node not right");
    mu_assert(new_list->first->next->value == list1->first->next->value, "2nd node not right");
    mu_assert(new_list->first->next->next->value == list1->first->next->next->value, "3rd node not right");
    mu_assert(new_list->last->value == list2->last->value, "6th node not right");
    mu_assert(new_list->last->prev->value == list2->last->prev->value, "5st node not right");
    mu_assert(new_list->last->prev->prev->value == list2->first->value, "4st node not right");
    List_destroy(new_list);
    List_clear_destroy(list1);
    List_clear_destroy(list2);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);
    mu_run_test(test_copy);
    mu_run_test(test_concat);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
