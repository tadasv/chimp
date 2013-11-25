#include <gtest/gtest.h>

#include <ch_list.h>
#include <ch_string.h>
#include <ch_hash_table.h>
#include <ch_row.h>
#include <ch_table.h>


TEST(ch_hash_table_test, ch_hash_table_test_init)
{
    ch_hash_table_t ht;

    ch_hash_table_init(&ht, 12);
    ch_hash_table_free(&ht);
}


TEST(ch_hash_table_test, ch_hash_table_multi_init)
{
    ch_hash_table_multi_t ht;
    ch_hash_table_multi_init(&ht, 10);
    ch_hash_table_multi_free(&ht);
}


TEST(ch_hash_table_test, ch_hash_table_multi_add)
{
    ch_hash_table_multi_t ht;
    int data = 1;
    int data2 = 2;

    /* one bucket so that we always get collisions */
    ch_hash_table_multi_init(&ht, 1);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == NULL);
    ch_hash_table_multi_add(&ht, (void*)"k1", 2, &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data);
    ch_hash_table_multi_add(&ht, (void*)"k2", 2, &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k2", 2) == &data2);
    ch_hash_table_multi_add(&ht, (void*)"k1", 2, &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k2", 2) == &data2);
    ch_hash_table_multi_free(&ht);

    /* same thing with more buckets */
    ch_hash_table_multi_init(&ht, 10);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == NULL);
    ch_hash_table_multi_add(&ht, (void*)"k1", 2, &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data);
    ch_hash_table_multi_add(&ht, (void*)"k2", 2, &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k2", 2) == &data2);
    ch_hash_table_multi_add(&ht, (void*)"k1", 2, &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k1", 2) == &data2);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"k2", 2) == &data2);
    ch_hash_table_multi_free(&ht);
}


TEST(ch_hash_table_test, ch_hash_table_delete)
{
    ch_hash_table_t ht;
    int data = 123;

    ch_hash_table_init(&ht, 10);
    ch_hash_table_add(&ht, (void*)"key", 3, &data);
    ASSERT_TRUE(ch_hash_table_find(&ht, (void*)"key", 3) == &data);
    ch_hash_table_delete(&ht, (void*)"key", 3);
    ASSERT_TRUE(ch_hash_table_find(&ht, (void*)"key", 3) == NULL);
    ch_hash_table_delete(&ht, (void*)"key2", 4);
    ch_hash_table_free(&ht);
}


TEST(ch_hash_table_test, ch_hash_table_multi_delete)
{
    ch_hash_table_multi_t ht;
    int data = 123;

    /* size one, to test removal on collisions */
    ch_hash_table_multi_init(&ht, 1);
    ch_hash_table_multi_add(&ht, (void*)"key", 3, &data);
    ch_hash_table_multi_add(&ht, (void*)"key2", 4, &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key", 3) == &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key2", 4) == &data);
    ch_hash_table_multi_delete(&ht, (void*)"key", 3);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key", 3) == NULL);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key2", 4) == &data);
    ch_hash_table_multi_free(&ht);

    ch_hash_table_multi_init(&ht, 20);
    ch_hash_table_multi_add(&ht, (void*)"key", 3, &data);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key", 3) == &data);
    ch_hash_table_multi_delete(&ht, (void*)"key", 3);
    ASSERT_TRUE(ch_hash_table_multi_find(&ht, (void*)"key", 3) == NULL);
    ch_hash_table_multi_delete(&ht, (void*)"key2", 4);
    ch_hash_table_multi_free(&ht);
}


TEST(ch_hash_table_test, ch_hash_table_add)
{
    ch_hash_table_t ht;
    int data = 1;
    int data2 = 2;

    ch_hash_table_init(&ht, 12);
    ASSERT_TRUE(ch_hash_table_find(&ht, (void*)"123", 3) == NULL);
    ch_hash_table_add(&ht, (void*)"123", 3, &data);
    ASSERT_TRUE(ch_hash_table_find(&ht, (void*)"123", 3) == &data);
    ch_hash_table_add(&ht, (void*)"123", 3, &data2);
    ASSERT_TRUE(ch_hash_table_find(&ht, (void*)"123", 3) == &data2);
    ch_hash_table_free(&ht);
}


TEST(ch_list_test, ch_list_init)
{
    ch_list_t list, list2;
    int data = 3;
    ch_list_init_empty(&list);
    ASSERT_TRUE(list.head == NULL);
    ASSERT_TRUE(list.tail == NULL);
    ch_list_free(&list);

    ch_list_init(&list2, &data);
    ASSERT_TRUE(list2.head == list2.tail);
    ASSERT_TRUE(list2.head->prev == NULL);
    ASSERT_TRUE(list2.head->next == NULL);
    ASSERT_TRUE(list2.head->data == &data);
    ch_list_free(&list2);
}


TEST(ch_list_test, ch_list_append)
{
    ch_list_t list;
    int data1 = 1;
    int data2 = 2;

    ch_list_init(&list, &data1);
    ch_list_append(&list, &data2);
    ch_list_append(&list, &data1);

    ASSERT_TRUE(list.head->data == &data1);
    ASSERT_TRUE(list.head->next->data == &data2);
    ASSERT_TRUE(list.head->next->next->data == &data1);

    ASSERT_TRUE(list.tail->data == &data1);
    ASSERT_TRUE(list.tail->prev->data == &data2);
    ASSERT_TRUE(list.tail->prev->prev->data == &data1);

    ASSERT_TRUE(list.head == list.tail->prev->prev);
    ASSERT_TRUE(list.head->next == list.tail->prev);
    ASSERT_TRUE(list.head->next->next == list.tail);

    ch_list_free(&list);
}


TEST(ch_str_test, ch_str_init)
{
    ch_str_t str1;
    ch_str_t str2;

    ch_str_init(&str1, "test");
    ASSERT_TRUE(strcmp(str1.data, "test") == 0);
    ASSERT_TRUE(str1.len == 4);
    ASSERT_TRUE(str1.size == 5);
    ch_str_free(&str1);

    ch_str_init_alloc(&str2, 32);
    ASSERT_TRUE(str2.len == 0);
    ASSERT_TRUE(str2.size == 32);
    ASSERT_TRUE(str2.data != NULL);
    ch_str_free(&str2);
}


TEST(ch_str_test, ch_str_cat)
{
    ch_str_t str;

    ch_str_init_alloc(&str, 10);
    ch_str_cat(&str, "123456");
    ASSERT_TRUE(strcmp(str.data, "123456") == 0);
    ch_str_cat(&str, "789");
    ASSERT_TRUE(str.len == 9);
    ASSERT_TRUE(str.size == 10);
    ASSERT_TRUE(strcmp(str.data, "123456789") == 0);
    ch_str_cat(&str, "testas");
    ASSERT_TRUE(strcmp(str.data, "123456789testas") == 0);
    ASSERT_TRUE(str.len == 15);
    ASSERT_TRUE(str.size == 32);
    ch_str_free(&str);
}


TEST(ch_str_test, ch_str_lcat)
{
    ch_str_t str;

    ch_str_init_alloc(&str, 10);
    ch_str_lcat(&str, "123456", 6);
    ASSERT_TRUE(strcmp(str.data, "123456") == 0);
    ch_str_lcat(&str, "789", 3);
    ASSERT_TRUE(str.len == 9);
    ASSERT_TRUE(str.size == 10);
    ASSERT_TRUE(strcmp(str.data, "123456789") == 0);
    ch_str_lcat(&str, "testas", 6);
    ASSERT_TRUE(strcmp(str.data, "123456789testas") == 0);
    ASSERT_TRUE(str.len == 15);
    ASSERT_TRUE(str.size == 32);
    ch_str_free(&str);
}


TEST(ch_str_test, ch_str_to_long)
{
    long result;

    ASSERT_NE(0, ch_str_to_long(NULL, &result));
    ASSERT_NE(0, ch_str_to_long("", &result));
    ASSERT_NE(0, ch_str_to_long("asf", &result));
    ASSERT_EQ(0, ch_str_to_long(" -1", &result));
    ASSERT_EQ(-1, result);
    ASSERT_NE(0, ch_str_to_long("10 ", &result));
    ASSERT_EQ(0, ch_str_to_long("10", &result));
    ASSERT_EQ(10, result);
}


TEST(ch_str_test, ch_str_to_double)
{
    double result;

    ASSERT_NE(0, ch_str_to_double(NULL, &result));
    ASSERT_NE(0, ch_str_to_double("", &result));
    ASSERT_NE(0, ch_str_to_double("asf", &result));
    ASSERT_EQ(0, ch_str_to_double(" -1", &result));
    ASSERT_TRUE(-1 == result);
    ASSERT_NE(0, ch_str_to_double("10 ", &result));
    ASSERT_EQ(0, ch_str_to_double("10", &result));
    ASSERT_TRUE(10 == result);
    ASSERT_EQ(0, ch_str_to_double("1.5", &result));
    ASSERT_TRUE(1.5 == result);
}


TEST(ch_str_test, ch_str_printf)
{
    ch_str_t str;

    ch_str_init_alloc(&str, 2);
    ch_str_printf(&str, "1");
    ASSERT_TRUE(strcmp(str.data, "1") == 0);
    ch_str_printf(&str, "test %d", 2);
    ASSERT_TRUE(strcmp(str.data, "1test 2") == 0);
    ch_str_free(&str);
}


TEST(ch_row_test, ch_row_init)
{
    ch_row_t row;
    ch_row_init(&row, 10);
    ch_row_free(&row);
}


TEST(ch_row_test, ch_row_get_set)
{
    int i;
    double value;
    ch_row_t row;
    ch_row_init(&row, 5);

    for (i = 0; i < 3; i++) {
        ASSERT_EQ(0, ch_row_set(&row, i, (double)i));
    }

    for (i = 0; i < row.ncols; i++) {
        ASSERT_EQ(0, ch_row_get(&row, i, &value));
        if (i >= 3) {
            ASSERT_TRUE(value == 0.0);
        } else {
            ASSERT_TRUE(value == i);
        }
    }

    ASSERT_EQ(-1, ch_row_get(&row, -1, &value));
    ASSERT_EQ(-1, ch_row_get(&row, 10, &value));
    ASSERT_EQ(-1, ch_row_set(&row, -1, value));
    ASSERT_EQ(-1, ch_row_set(&row, 10, value));
    ASSERT_EQ(-1, ch_row_set(&row, 5, value));
    /* test for one-off error */
    ASSERT_EQ(-1, ch_row_set(&row, 5, value));
    ASSERT_EQ(-1, ch_row_get(&row, 10, &value));

    ch_row_free(&row);
}


TEST(ch_table_test, ch_table_init)
{
    ch_table_t table;
    ch_table_init(&table, 10, 10);
    ASSERT_EQ(0, table.nrows);
    ch_table_free(&table);
}


TEST(ch_table_test, ch_table_get_set)
{
    ch_table_t table;
    double value = 0;
    off_t i;

    ch_table_init(&table, 2, 2);
    ASSERT_EQ(0, table.nrows);

    ASSERT_EQ(-1, ch_table_get(&table, 0, 0, &value));
    ASSERT_EQ(-1, ch_table_get(&table, 1, 1, &value));
    ASSERT_EQ(-1, ch_table_get(&table, 10, 10, &value));

    ASSERT_EQ(0, ch_table_set(&table, 0, 0, 10));
    ASSERT_EQ(-1, ch_table_set(&table, 0, 2, 20));
    ASSERT_EQ(1, table.nrows);

    ASSERT_EQ(0, ch_table_get(&table, 0, 0, &value));
    ASSERT_TRUE(value == 10);
    /* if row does not exist when we set a value it will be created
     * automatically and filled with zeros */
    ASSERT_EQ(0, ch_table_get(&table, 0, 1, &value));
    ASSERT_TRUE(value == 0);
    ASSERT_EQ(-1, ch_table_get(&table, 0, 2, &value));

    /* try expanding index size */
    for (i = 0; i < 10; i++) {
        ASSERT_EQ(0, ch_table_set(&table, i, 0, 1));
        ASSERT_EQ(0, ch_table_set(&table, i, 1, 2));
        /* can't expand columns */
        ASSERT_EQ(-1, ch_table_set(&table, i, 2, 3));
    }
    ASSERT_EQ(10, table.nrows);

    for (i = 0; i < 10; i++) {
        ASSERT_EQ(0, ch_table_get(&table, i, 0, &value));
        ASSERT_TRUE(value == 1);
        ASSERT_EQ(0, ch_table_get(&table, i, 1, &value));
        ASSERT_TRUE(value == 2);
    }

    ch_table_free(&table);
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
