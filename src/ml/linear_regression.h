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
#ifndef CH_INCLUDE_GUARD_7B0EF209_754A_41C5_BD80_4A67A824EA5F
#define CH_INCLUDE_GUARD_7B0EF209_754A_41C5_BD80_4A67A824EA5F

#include <cstdint>
#include <string>
#include <vector>
#include <armadillo>
#include "db/abstract_dataset.h"
#include "ml/abstract_model.h"

namespace chimp {
namespace ml {
namespace model {

class LinearRegression : public AbstractModel {
    public:
        class BuildInput : public AbstractModelInput {
            public:
                chimp::db::dataset::AbstractDataset *dataset;
                std::vector<uint32_t> feature_columns;
                uint32_t response_column;
        };

        class PredictionInput : public AbstractModelInput {
            public:
                std::vector<double> data;
        };

        class PredictionResult : public AbstractModelResult {
            public:
                msgpack_sbuffer *ToMessagePack();
                std::vector<double> predictions;
        };

        int Build(const AbstractModelInput *input);
        std::shared_ptr<AbstractModelResult> Predict(const AbstractModelInput *input) const;

    private:
        arma::vec parameters_;
};

}; // namespace model
}; // namespace ml
}; // namespace chimp

#endif /* end of include guard */
