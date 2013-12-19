#include <gtest/gtest.h>

#include "db/dataset.h"


TEST(Dataset, Constructor)
{
    chimp::db::Dataset dataset("iris", 2);
    chimp::db::dataset::Dataset dataset2("iris", 2);
}


TEST(Dataset, GetName)
{
    chimp::db::Dataset dataset("iris.csv", 2);
    ASSERT_EQ("iris.csv", dataset.GetName());

    chimp::db::dataset::Dataset dataset2("iris.csv", 2);
    ASSERT_EQ("iris.csv", dataset2.GetName());
}


TEST(Dataset, Resize)
{
    chimp::db::dataset::Dataset dataset("iris.csv", 2);
    chimp::db::dataset::Dimensions dims;
    dims = dataset.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(2, dims.cols);

    dataset.Resize(4, 2);
    dims = dataset.GetDimensions();
    ASSERT_EQ(4, dims.rows);
    ASSERT_EQ(2, dims.cols);

    dataset.Resize(8, 8);
    dims = dataset.GetDimensions();
    ASSERT_EQ(8, dims.rows);
    ASSERT_EQ(8, dims.cols);
}


TEST(Dataset, GetDimensions)
{
    chimp::db::Dataset dataset("iris.csv", 2);

    chimp::db::dataset::Dimensions dims = dataset.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(2, dims.cols);

    dataset.SetItem(0, 0, 10);
    dims = dataset.GetDimensions();
    ASSERT_EQ(1, dims.rows);
    ASSERT_EQ(2, dims.cols);

    dataset.SetItem(1, 0, 10);
    dims = dataset.GetDimensions();
    ASSERT_EQ(2, dims.rows);
    ASSERT_EQ(2, dims.cols);


    chimp::db::dataset::Dataset dataset2("iris.csv", 2);
    dims = dataset2.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(2, dims.cols);
}


TEST(Dataset, GetSet)
{
    chimp::db::Dataset dataset("iris", 2);
    double value;

    ASSERT_EQ(0, dataset.NumRows());

    ASSERT_EQ(-1, dataset.GetItem(0, 0, &value));
    ASSERT_EQ(-1, dataset.GetItem(1, 1, &value));
    ASSERT_EQ(-1, dataset.GetItem(10, 10, &value));

    ASSERT_EQ(0, dataset.SetItem(0, 0, 10));
    ASSERT_EQ(-1, dataset.SetItem(0, 2, 20));
    ASSERT_EQ(1, dataset.NumRows());

    ASSERT_EQ(0, dataset.GetItem(0, 0, &value));
    ASSERT_DOUBLE_EQ(10, value);
    /* if row does not exist when we set a value it will be created
     * automatically and filled with zeros */
    ASSERT_EQ(0, dataset.GetItem(0, 1, &value));
    ASSERT_NEAR(0, value, 1e-300);
    ASSERT_EQ(-1, dataset.GetItem(0, 2, &value));

    /* try expanding index size */
    for (uint32_t i = 0; i < 10; i++) {
        ASSERT_EQ(0, dataset.SetItem(i, 0, 1));
        ASSERT_EQ(0, dataset.SetItem(i, 1, 2));
        /* can't expand columns */
        ASSERT_EQ(-1, dataset.SetItem(i, 2, 3));
    }
    ASSERT_EQ(10, dataset.NumRows());

    for (uint32_t i = 0; i < 10; i++) {
        ASSERT_EQ(0, dataset.GetItem(i, 0, &value));
        ASSERT_DOUBLE_EQ(1, value);
        ASSERT_EQ(0, dataset.GetItem(i, 1, &value));
        ASSERT_DOUBLE_EQ(2, value);
    }


    chimp::db::dataset::Dataset dataset2("iris", 2);
    chimp::db::dataset::Dimensions dims = dataset2.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(2, dims.cols);

    ASSERT_EQ(-1, dataset2.GetItem(0, 0, &value));
    ASSERT_EQ(-1, dataset2.GetItem(1, 1, &value));
    ASSERT_EQ(-1, dataset2.GetItem(10, 10, &value));

    // can't set item if it's there is no room for it.
    ASSERT_EQ(-1, dataset2.SetItem(0, 0, 10));
    dataset2.Resize(1, dims.cols);

    ASSERT_EQ(0, dataset2.SetItem(0, 0, 10));
    ASSERT_EQ(-1, dataset2.SetItem(0, 2, 20));
    dims = dataset2.GetDimensions();
    ASSERT_EQ(1, dims.rows);

    ASSERT_EQ(0, dataset2.GetItem(0, 0, &value));
    ASSERT_DOUBLE_EQ(10, value);
    /* if row does not exist when we set a value it will be created
     * automatically and filled with zeros */
    ASSERT_EQ(0, dataset2.GetItem(0, 1, &value));
    ASSERT_NEAR(0, value, 1e-300);
    ASSERT_EQ(-1, dataset2.GetItem(0, 2, &value));
}
