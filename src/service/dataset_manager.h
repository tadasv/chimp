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
#ifndef CH_INCLUDE_GUARD_447236A5_2FAA_41EE_AB2A_CD40F1ACF5C7
#define CH_INCLUDE_GUARD_447236A5_2FAA_41EE_AB2A_CD40F1ACF5C7

#include <map>
#include <string>
#include <memory>

#include "db/dataset.h"


namespace chimp {
namespace service {


class AbstractDatasetManager {
    public:
        virtual ~AbstractDatasetManager();

        virtual int AddDataset(std::shared_ptr<chimp::db::Dataset> dataset) = 0;
        virtual int DatasetExists(std::string name) = 0;
        virtual std::shared_ptr<chimp::db::Dataset> FindDataset(std::string name) = 0;
    protected:
        AbstractDatasetManager();
};


class DatasetManagerImpl : public AbstractDatasetManager {
    public:
        int AddDataset(std::shared_ptr<chimp::db::Dataset> dataset);
        int DatasetExists(std::string name);
        std::shared_ptr<chimp::db::Dataset> FindDataset(std::string name);
    private:
        std::map<std::string, std::shared_ptr<chimp::db::Dataset>> datasets_;
};

// Singleton factory
class DatasetManager {
    public:
        static AbstractDatasetManager *GetInstance();
    protected:
        DatasetManager();
    private:
        static std::unique_ptr<AbstractDatasetManager> instance_;
};

}
}


#endif /* end of include guard */
