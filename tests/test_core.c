#include <check.h>

#include <ch_list.h>
#include <ch_string.h>


START_TEST(test_ch_list_init)
{
    ch_list_t list, list2;
    int data = 3;
    ch_list_init_empty(&list);
    ck_assert(list.head == NULL);
    ck_assert(list.tail == NULL);
    ch_list_free(&list);

    ch_list_init(&list2, &data);
    ck_assert(list2.head == list2.tail);
    ck_assert(list2.head->prev == NULL);
    ck_assert(list2.head->next == NULL);
    ck_assert(list2.head->data == &data);
    ch_list_free(&list2);
}
END_TEST


START_TEST(test_ch_list_append)
{
    ch_list_t list;
    int data1 = 1;
    int data2 = 2;

    ch_list_init(&list, &data1);
    ch_list_append(&list, &data2);
    ch_list_append(&list, &data1);

    ck_assert(list.head->data == &data1);
    ck_assert(list.head->next->data == &data2);
    ck_assert(list.head->next->next->data == &data1);

    ck_assert(list.tail->data == &data1);
    ck_assert(list.tail->prev->data == &data2);
    ck_assert(list.tail->prev->prev->data == &data1);

    ck_assert(list.head == list.tail->prev->prev);
    ck_assert(list.head->next == list.tail->prev);
    ck_assert(list.head->next->next == list.tail);

    ch_list_free(&list);
}
END_TEST


START_TEST(test_ch_str_init)
{
    ch_str_t str1;
    ch_str_t str2;

    ch_str_init(&str1, "test");
    ck_assert(strcmp(str1.data, "test") == 0);
    ck_assert(str1.len == 4);
    ck_assert(str1.size == 5);
    ch_str_free(&str1);

    ch_str_init_alloc(&str2, 32);
    ck_assert(str2.len == 0);
    ck_assert(str2.size == 32);
    ck_assert(str2.data != NULL);
    ch_str_free(&str2);
}
END_TEST


START_TEST(test_ch_str_cat)
{
    ch_str_t str;

    ch_str_init_alloc(&str, 10);
    ch_str_cat(&str, "123456");
    ck_assert(strcmp(str.data, "123456") == 0);
    ch_str_cat(&str, "789");
    ck_assert(str.len == 9);
    ck_assert(str.size == 10);
    ck_assert(strcmp(str.data, "123456789") == 0);
    ch_str_cat(&str, "testas");
    ck_assert(strcmp(str.data, "123456789testas") == 0);
    ck_assert(str.len == 15);
    ck_assert(str.size == 20);
    ch_str_free(&str);
}
END_TEST


int main(int argc, const char *argv[])
{
    Suite *s = suite_create("core");

    TCase *ch_list_test_case = tcase_create("ch_list");
    tcase_add_test(ch_list_test_case, test_ch_list_init);
    tcase_add_test(ch_list_test_case, test_ch_list_append);

    TCase *ch_str_test_case = tcase_create("ch_str");
    tcase_add_test(ch_str_test_case, test_ch_str_init);
    tcase_add_test(ch_str_test_case, test_ch_str_cat);

    /* Add test cases here */
    suite_add_tcase(s, ch_list_test_case);
    suite_add_tcase(s, ch_str_test_case);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    srunner_free(sr);
    return 0;
}
