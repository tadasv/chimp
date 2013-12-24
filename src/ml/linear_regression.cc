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
#ifndef CH_INCLUDE_GUARD_146668BD_C4A4_41B1_B54C_39DCA233B433
#define CH_INCLUDE_GUARD_146668BD_C4A4_41B1_B54C_39DCA233B433

#include <cassert>
#include <iostream>
#include "db/dataset.h"
#include "ml/linear_regression.h"

namespace chimp {
namespace ml {
namespace model {

int LinearRegression::Build(const AbstractModelInput *input)
{
    assert(input);

    const BuildInput *build_input = dynamic_cast<const BuildInput*>(input);
    chimp::db::dataset::Dataset *dataset = dynamic_cast<chimp::db::dataset::Dataset*>(build_input->dataset);
    auto n_cols = build_input->feature_columns.size();
    auto dims = build_input->dataset->GetDimensions();

    // TODO handle armadillo exceptions
    arma::mat features;
    // add extra column for bias
    features.set_size(dims.rows, n_cols + 1);
    for (unsigned int i = 0; i < n_cols; ++i) {
        features(arma::span::all, i + 1) = dataset->RawData().col(build_input->feature_columns[i]);
    }
    // set bias column to 1
    features.col(0).fill(1);

    arma::colvec responses = dataset->RawData().col(build_input->response_column);
    // QR factorization
    arma::mat Q, R;
    arma::qr(Q, R, features);
    arma::solve(parameters_, R, arma::trans(Q) * responses);

    return 0;
}


std::shared_ptr<AbstractModelResult> LinearRegression::Predict(const AbstractModelInput *input) const
{
    assert(input);
    // TODO handle armadillo exceptions

    const PredictionInput *pinput = dynamic_cast<const PredictionInput*>(input);

    assert(pinput->data.size() % (parameters_.n_rows - 1) == 0);
    auto n_rows = pinput->data.size() / (parameters_.n_rows - 1);

    arma::mat data(n_rows, parameters_.n_rows);
    uint32_t starting_row = 0;
    uint32_t starting_column = 1;
    for (unsigned int i = 0; i < pinput->data.size(); i++) {
        data(starting_row, starting_column++) = pinput->data[i];
        if (starting_column == data.n_cols) {
            starting_column = 1;
            starting_row++;
        }
    }
    data.col(0).fill(1);

    arma::vec predictions = data * parameters_;
    PredictionResult *result = new PredictionResult();
    for (unsigned int i = 0; i < predictions.n_rows; ++i) {
        result->predictions.push_back(predictions.row(i));
    }

    return std::shared_ptr<AbstractModelResult>(result);
}


msgpack_sbuffer *LinearRegression::PredictionResult::ToMessagePack()
{
    return NULL;
}


}; // namespace model
}; // namespace ml
}; // namespace chimp

#endif /* end of include guard */
