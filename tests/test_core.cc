#include <gtest/gtest.h>

#include <ch_row.h>
#include <ch_table.h>


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
