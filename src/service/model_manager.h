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
#ifndef CH_INCLUDE_GUARD_63B27B54_57AA_439D_B249_C62471B7443E
#define CH_INCLUDE_GUARD_63B27B54_57AA_439D_B249_C62471B7443E

#include <string>
#include <map>
#include <memory>
#include "ml/abstract_model.h"

namespace chimp {
namespace service {


class AbstractModelManager {
    public:
        virtual ~AbstractModelManager();

        virtual int AddModel(std::shared_ptr<chimp::ml::model::AbstractModel> model) = 0;
        virtual int ModelExists(std::string name) const = 0;
        virtual std::shared_ptr<chimp::ml::model::AbstractModel> FindModel(std::string name) = 0;
};


class ModelManagerImpl : public AbstractModelManager {
    public:
        int AddModel(std::shared_ptr<chimp::ml::model::AbstractModel> model);
        int ModelExists(std::string name) const;
        std::shared_ptr<chimp::ml::model::AbstractModel> FindModel(std::string name);
    private:
        std::map<std::string, std::shared_ptr<chimp::ml::model::AbstractModel>> models_;
};


// Singleton factory
class ModelManager {
    public:
        static AbstractModelManager *GetInstance();
    protected:
        ModelManager();
    private:
        static std::unique_ptr<AbstractModelManager> instance_;
};

}; // namespace service
}; // namespace chimp

#endif /* end of include guard */
