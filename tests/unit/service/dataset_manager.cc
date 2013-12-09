#include <gtest/gtest.h>

#include "service/dataset_manager.h"


TEST(DatasetManager, Constructor)
{
    chimp::service::DatasetManager manager;
}


TEST(DatasetManager, AddDataset)
{
    chimp::service::DatasetManager manager;

    std::shared_ptr<chimp::db::Dataset> dataset(new chimp::db::Dataset("iris", 2));
    std::shared_ptr<chimp::db::Dataset> dataset2(new chimp::db::Dataset("iris", 2));

    EXPECT_EQ(0, manager.AddDataset(dataset));
    EXPECT_EQ(-1, manager.AddDataset(dataset2));
}


TEST(DatasetManager, DatasetExists)
{
    chimp::service::DatasetManager manager;
    std::shared_ptr<chimp::db::Dataset> dataset(new chimp::db::Dataset("iris", 2));

    EXPECT_EQ(0, manager.DatasetExists("iris"));
    EXPECT_EQ(0, manager.AddDataset(dataset));
    EXPECT_EQ(1, manager.DatasetExists("iris"));
}


TEST(DatasetManager, FindDataset)
{
    chimp::service::DatasetManager manager;
    std::shared_ptr<chimp::db::Dataset> dataset(new chimp::db::Dataset("iris", 2));

    EXPECT_EQ(std::shared_ptr<chimp::db::Dataset>(NULL), manager.FindDataset("iris"));
    EXPECT_EQ(0, manager.AddDataset(dataset));
    auto result = manager.FindDataset("iris");
    ASSERT_EQ(result->GetName(), "iris");
    ASSERT_EQ(result, dataset);
}
