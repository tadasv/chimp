#include <gtest/gtest.h>

#include "service/dataset_manager.h"
#include "db/dataset.h"


TEST(DatasetManagerImpl, Constructor)
{
    chimp::service::DatasetManagerImpl manager;
}


TEST(DatasetManagerImpl, AddDataset)
{
    chimp::service::DatasetManagerImpl manager;

    std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset(new chimp::db::dataset::Dataset("iris", 2));
    std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset2(new chimp::db::dataset::Dataset("iris", 2));

    EXPECT_EQ(0, manager.AddDataset(dataset));
    EXPECT_EQ(-1, manager.AddDataset(dataset2));
}


TEST(DatasetManagerImpl, DatasetExists)
{
    chimp::service::DatasetManagerImpl manager;
    std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset(new chimp::db::dataset::Dataset("iris", 2));

    EXPECT_EQ(0, manager.DatasetExists("iris"));
    EXPECT_EQ(0, manager.AddDataset(dataset));
    EXPECT_EQ(1, manager.DatasetExists("iris"));
}


TEST(DatasetManagerImpl, FindDataset)
{
    chimp::service::DatasetManagerImpl manager;
    std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset(new chimp::db::dataset::Dataset("iris", 2));

    EXPECT_EQ(std::shared_ptr<chimp::db::dataset::Dataset>(NULL), manager.FindDataset("iris"));
    EXPECT_EQ(0, manager.AddDataset(dataset));
    auto result = manager.FindDataset("iris");
    ASSERT_EQ(result->GetName(), "iris");
    ASSERT_EQ(result, dataset);
}
