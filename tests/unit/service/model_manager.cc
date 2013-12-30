#include <gtest/gtest.h>

#include "service/model_manager.h"
#include "ml/linear_regression.h"


TEST(ModelManagerImpl, Constructor)
{
    chimp::service::ModelManagerImpl manager;
}


TEST(ModelManagerImpl, AddModel)
{
    chimp::service::ModelManagerImpl manager;

    auto lm1 = new chimp::ml::model::LinearRegression();
    lm1->SetName("lm1");
    std::shared_ptr<chimp::ml::model::AbstractModel> model1(lm1);
    auto lm2 = new chimp::ml::model::LinearRegression();
    lm2->SetName("lm1");
    std::shared_ptr<chimp::ml::model::AbstractModel> model2(lm2);

    EXPECT_EQ(0, manager.AddModel(model1));
    EXPECT_EQ(-1, manager.AddModel(model2));
}


TEST(ModelManagerImpl, ModelExists)
{
    chimp::service::ModelManagerImpl manager;
    std::shared_ptr<chimp::ml::model::AbstractModel> model(
        new chimp::ml::model::LinearRegression());
    model->SetName("lm1");

    EXPECT_EQ(0, manager.ModelExists("lm1"));
    EXPECT_EQ(0, manager.AddModel(model));
    EXPECT_EQ(1, manager.ModelExists("lm1"));
}


TEST(ModelManagerImpl, FindModel)
{
    chimp::service::ModelManagerImpl manager;
    std::shared_ptr<chimp::ml::model::AbstractModel> model(
       new chimp::ml::model::LinearRegression());
    model->SetName("lm1");

    EXPECT_EQ(std::shared_ptr<chimp::ml::model::AbstractModel>(NULL), manager.FindModel("lm1"));
    EXPECT_EQ(0, manager.AddModel(model));
    auto result = manager.FindModel("lm1");
    ASSERT_EQ(result->GetName(), "lm1");
    ASSERT_EQ(result, model);
}
