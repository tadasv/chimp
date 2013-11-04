#include <check.h>

#include <ch_list.h>
#include <ch_string.h>
#include <ch_hash_table.h>
#include <ch_row.h>
#include <ch_table.h>


START_TEST(test_ch_hash_table_init)
{
    ch_hash_table_t ht;

    ch_hash_table_init(&ht, 12);
    ch_hash_table_free(&ht);
}
END_TEST


START_TEST(test_ch_hash_table_multi_init)
{
    ch_hash_table_multi_t ht;
    ch_hash_table_multi_init(&ht, 10);
    ch_hash_table_multi_free(&ht);
}
END_TEST


START_TEST(test_ch_hash_table_multi_add)
{
    ch_hash_table_multi_t ht;
    int data = 1;
    int data2 = 2;

    /* one bucket so that we always get collisions */
    ch_hash_table_multi_init(&ht, 1);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == NULL);
    ch_hash_table_multi_add(&ht, "k1", 2, &data);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data);
    ch_hash_table_multi_add(&ht, "k2", 2, &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data);
    ck_assert(ch_hash_table_multi_find(&ht, "k2", 2) == &data2);
    ch_hash_table_multi_add(&ht, "k1", 2, &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k2", 2) == &data2);
    ch_hash_table_multi_free(&ht);

    /* same thing with more buckets */
    ch_hash_table_multi_init(&ht, 10);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == NULL);
    ch_hash_table_multi_add(&ht, "k1", 2, &data);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data);
    ch_hash_table_multi_add(&ht, "k2", 2, &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data);
    ck_assert(ch_hash_table_multi_find(&ht, "k2", 2) == &data2);
    ch_hash_table_multi_add(&ht, "k1", 2, &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k1", 2) == &data2);
    ck_assert(ch_hash_table_multi_find(&ht, "k2", 2) == &data2);
    ch_hash_table_multi_free(&ht);
}
END_TEST


START_TEST(test_ch_hash_table_add)
{
    ch_hash_table_t ht;
    int data = 1;
    int data2 = 2;

    ch_hash_table_init(&ht, 12);
    ck_assert(ch_hash_table_find(&ht, "123", 3) == NULL);
    ch_hash_table_add(&ht, "123", 3, &data);
    ck_assert(ch_hash_table_find(&ht, "123", 3) == &data);
    ch_hash_table_add(&ht, "123", 3, &data2);
    ck_assert(ch_hash_table_find(&ht, "123", 3) == &data2);
    ch_hash_table_free(&ht);
}
END_TEST


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
    ck_assert(str.size == 32);
    ch_str_free(&str);
}
END_TEST


START_TEST(test_ch_str_lcat)
{
    ch_str_t str;

    ch_str_init_alloc(&str, 10);
    ch_str_lcat(&str, "123456", 6);
    ck_assert(strcmp(str.data, "123456") == 0);
    ch_str_lcat(&str, "789", 3);
    ck_assert(str.len == 9);
    ck_assert(str.size == 10);
    ck_assert(strcmp(str.data, "123456789") == 0);
    ch_str_lcat(&str, "testas", 6);
    ck_assert(strcmp(str.data, "123456789testas") == 0);
    ck_assert(str.len == 15);
    ck_assert(str.size == 32);
    ch_str_free(&str);
}
END_TEST


START_TEST(test_ch_str_to_long)
{
    long result;

    ck_assert_int_ne(0, ch_str_to_long(NULL, &result));
    ck_assert_int_ne(0, ch_str_to_long("", &result));
    ck_assert_int_ne(0, ch_str_to_long("asf", &result));
    ck_assert_int_eq(0, ch_str_to_long(" -1", &result));
    ck_assert_int_eq(-1, result);
    ck_assert_int_ne(0, ch_str_to_long("10 ", &result));
    ck_assert_int_eq(0, ch_str_to_long("10", &result));
    ck_assert_int_eq(10, result);
}
END_TEST


START_TEST(test_ch_str_to_double)
{
    double result;

    ck_assert_int_ne(0, ch_str_to_double(NULL, &result));
    ck_assert_int_ne(0, ch_str_to_double("", &result));
    ck_assert_int_ne(0, ch_str_to_double("asf", &result));
    ck_assert_int_eq(0, ch_str_to_double(" -1", &result));
    ck_assert(-1 == result);
    ck_assert_int_ne(0, ch_str_to_double("10 ", &result));
    ck_assert_int_eq(0, ch_str_to_double("10", &result));
    ck_assert(10 == result);
    ck_assert_int_eq(0, ch_str_to_double("1.5", &result));
    ck_assert(1.5 == result);
}
END_TEST


START_TEST(test_ch_row_init)
{
    ch_row_t row;
    ch_row_init(&row, 10);
    ch_row_free(&row);
}
END_TEST


START_TEST(test_ch_row_get_set)
{
    int i;
    double value;
    ch_row_t row;
    ch_row_init(&row, 5);

    for (i = 0; i < 3; i++) {
        ck_assert_int_eq(0, ch_row_set(&row, i, (double)i));
    }

    for (i = 0; i < row.ncols; i++) {
        ck_assert_int_eq(0, ch_row_get(&row, i, &value));
        if (i >= 3) {
            ck_assert(value == 0.0);
        } else {
            ck_assert(value == i);
        }
    }

    ck_assert_int_eq(-1, ch_row_get(&row, -1, &value));
    ck_assert_int_eq(-1, ch_row_get(&row, 10, &value));
    ck_assert_int_eq(-1, ch_row_set(&row, -1, value));
    ck_assert_int_eq(-1, ch_row_set(&row, 10, value));
    ck_assert_int_eq(-1, ch_row_set(&row, 5, value));
    /* test for one-off error */
    ck_assert_int_eq(-1, ch_row_set(&row, 5, value));
    ck_assert_int_eq(-1, ch_row_get(&row, 10, &value));

    ch_row_free(&row);
}
END_TEST


START_TEST(test_ch_table_init)
{
    ch_table_t table;
    ch_table_init(&table, 10, 10);
    ck_assert_int_eq(0, table.nrows);
    ch_table_free(&table);
}
END_TEST


START_TEST(test_ch_table_get_set)
{
    ch_table_t table;
    double value = 0;
    off_t i;

    ch_table_init(&table, 2, 2);
    ck_assert_int_eq(0, table.nrows);

    ck_assert_int_eq(-1, ch_table_get(&table, 0, 0, &value));
    ck_assert_int_eq(-1, ch_table_get(&table, 1, 1, &value));
    ck_assert_int_eq(-1, ch_table_get(&table, 10, 10, &value));

    ck_assert_int_eq(0, ch_table_set(&table, 0, 0, 10));
    ck_assert_int_eq(-1, ch_table_set(&table, 0, 2, 20));
    ck_assert_int_eq(1, table.nrows);

    ck_assert_int_eq(0, ch_table_get(&table, 0, 0, &value));
    ck_assert(value == 10);
    /* if row does not exist when we set a value it will be created
     * automatically and filled with zeros */
    ck_assert_int_eq(0, ch_table_get(&table, 0, 1, &value));
    ck_assert(value == 0);
    ck_assert_int_eq(-1, ch_table_get(&table, 0, 2, &value));

    /* try expanding index size */
    for (i = 0; i < 10; i++) {
        ck_assert_int_eq(0, ch_table_set(&table, i, 0, 1));
        ck_assert_int_eq(0, ch_table_set(&table, i, 1, 2));
        /* can't expand columns */
        ck_assert_int_eq(-1, ch_table_set(&table, i, 2, 3));
    }
    ck_assert_int_eq(10, table.nrows);

    for (i = 0; i < 10; i++) {
        ck_assert_int_eq(0, ch_table_get(&table, i, 0, &value));
        ck_assert(value == 1);
        ck_assert_int_eq(0, ch_table_get(&table, i, 1, &value));
        ck_assert(value == 2);
    }

    ch_table_free(&table);
}
END_TEST


int main(int argc, const char *argv[])
{
    int number_failed;
    Suite *s = suite_create("core");

    TCase *ch_list_test_case = tcase_create("ch_list");
    tcase_add_test(ch_list_test_case, test_ch_list_init);
    tcase_add_test(ch_list_test_case, test_ch_list_append);

    TCase *ch_str_test_case = tcase_create("ch_str");
    tcase_add_test(ch_str_test_case, test_ch_str_init);
    tcase_add_test(ch_str_test_case, test_ch_str_cat);
    tcase_add_test(ch_str_test_case, test_ch_str_lcat);
    tcase_add_test(ch_str_test_case, test_ch_str_to_long);
    tcase_add_test(ch_str_test_case, test_ch_str_to_double);

    TCase *ch_hash_table_test_case = tcase_create("ch_hash_table");
    tcase_add_test(ch_hash_table_test_case, test_ch_hash_table_init);
    tcase_add_test(ch_hash_table_test_case, test_ch_hash_table_add);
    tcase_add_test(ch_hash_table_test_case, test_ch_hash_table_multi_init);
    tcase_add_test(ch_hash_table_test_case, test_ch_hash_table_multi_add);

    TCase *ch_row_test_case = tcase_create("ch_row");
    tcase_add_test(ch_row_test_case, test_ch_row_init);
    tcase_add_test(ch_row_test_case, test_ch_row_get_set);

    TCase *ch_table_test_case = tcase_create("ch_table");
    tcase_add_test(ch_table_test_case, test_ch_table_init);
    tcase_add_test(ch_table_test_case, test_ch_table_get_set);

    /* Add test cases here */
    suite_add_tcase(s, ch_list_test_case);
    suite_add_tcase(s, ch_str_test_case);
    suite_add_tcase(s, ch_hash_table_test_case);
    suite_add_tcase(s, ch_row_test_case);
    suite_add_tcase(s, ch_table_test_case);

    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : -1;
}
