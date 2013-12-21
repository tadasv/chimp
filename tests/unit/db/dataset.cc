#include <gtest/gtest.h>

#include "db/dataset.h"


TEST(Dataset, Constructor)
{
    chimp::db::dataset::Dataset dataset2("iris", 2);
}


TEST(Dataset, GetName)
{
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
    chimp::db::dataset::Dataset dataset2("iris.csv", 2);
    chimp::db::dataset::Dimensions dims;
    dims = dataset2.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(2, dims.cols);
}


TEST(Dataset, GetSet)
{
    double value;
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


TEST(Dataset, Append)
{
    chimp::db::dataset::Dataset dataset("iris", 4);
    chimp::db::dataset::Dimensions dims;
    std::vector<double> data;

    ASSERT_EQ(0, dataset.Append(data));
    dims = dataset.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(4, dims.cols);

    data.push_back(1);
    ASSERT_EQ(-1, dataset.Append(data));
    dims = dataset.GetDimensions();
    ASSERT_EQ(0, dims.rows);
    ASSERT_EQ(4, dims.cols);

    data.push_back(2);
    data.push_back(3);
    data.push_back(4);
    ASSERT_EQ(0, dataset.Append(data));
    dims = dataset.GetDimensions();
    ASSERT_EQ(1, dims.rows);
    ASSERT_EQ(4, dims.cols);

    for (int i = 0; i < 16; i++) {
        data.push_back(3.3);
    }
    ASSERT_EQ(0, dataset.Append(data));
    dims = dataset.GetDimensions();
    ASSERT_EQ(6, dims.rows);
    ASSERT_EQ(4, dims.cols);
}
