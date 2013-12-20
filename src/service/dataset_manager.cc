/**
 * Copyright (C) 2013 Tadas Vilkeliskis <vilkeliskis.t@gmail.com>
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "service/dataset_manager.h"


namespace chimp {
namespace service {

std::unique_ptr<AbstractDatasetManager> DatasetManager::instance_ = NULL;


AbstractDatasetManager::AbstractDatasetManager()
{
}


AbstractDatasetManager::~AbstractDatasetManager()
{
}


DatasetManager::DatasetManager()
{
}


AbstractDatasetManager *DatasetManager::GetInstance()
{
    if (instance_ == NULL) {
        instance_.reset(new DatasetManagerImpl());
    }

    return instance_.get();
}


int DatasetManagerImpl::AddDataset(std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset)
{
    std::string dataset_name = dataset->GetName();
    if (DatasetExists(dataset_name)) {
        return -1;
    }

    datasets_[dataset_name] = dataset;
    return 0;
}


int DatasetManagerImpl::DatasetExists(std::string name)
{
    auto iterator = datasets_.find(name);
    if (iterator == datasets_.end()) {
        return 0;
    }

    return 1;
}


const std::map<std::string, std::shared_ptr<chimp::db::dataset::AbstractDataset>> &DatasetManagerImpl::GetDatasets() const
{
    return datasets_;
}


std::shared_ptr<chimp::db::dataset::AbstractDataset> DatasetManagerImpl::FindDataset(std::string name)
{
    auto iterator = datasets_.find(name);
    if (iterator == datasets_.end()) {
        return NULL;
    }

    return iterator->second;
}


}
}
