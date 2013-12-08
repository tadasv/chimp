#include <gtest/gtest.h>

#include "db/row.h"


TEST(DatasetRow, Constructor)
{
    chimp::db::DatasetRow row(10);
}


TEST(DatasetRow, SetGetColumn)
{
    chimp::db::DatasetRow row(10);
    double value = -1;

    EXPECT_EQ(-1, row.SetColumn(10, 33));
    EXPECT_EQ(-1, row.GetColumn(10, &value));
    EXPECT_EQ(0, row.SetColumn(9, 33));
    EXPECT_EQ(0, row.GetColumn(9, &value));
    EXPECT_DOUBLE_EQ(33, value);
    EXPECT_EQ(0, row.GetColumn(0, &value));
    EXPECT_NEAR(0.0, value, 1e-300);
}


TEST(DatasetRow, NumColumns)
{
    chimp::db::DatasetRow row(10);
    ASSERT_EQ(10, row.NumColumns());
}
