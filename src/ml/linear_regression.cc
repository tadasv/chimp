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
#include "core/logging.h"
#include "db/dataset.h"
#include "ml/linear_regression.h"
#include "service/dataset_manager.h"

namespace chimp {
namespace ml {
namespace model {

int LinearRegression::Build(const AbstractModelInput *input)
{
    assert(input);

    const BuildInput *build_input = dynamic_cast<const BuildInput*>(input);
    chimp::db::dataset::Dataset *dataset;

    // TODO need to fix this, we currently set dataset in unit tests
    // but pass in dataset name via the api.
    if (build_input->dataset) {
        dataset = dynamic_cast<chimp::db::dataset::Dataset*>(build_input->dataset);
    } else {
        auto manager = chimp::service::DatasetManager::GetInstance();
        if (!manager->DatasetExists(build_input->dataset_name)) {
            return -1;
        }

        dataset = dynamic_cast<chimp::db::dataset::Dataset*>(manager->FindDataset(build_input->dataset_name).get());
    }
    auto n_cols = build_input->feature_columns.size();
    auto dims = dataset->GetDimensions();

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

    CH_LOG_DEBUG("parameters: " << parameters_);

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


int LinearRegression::BuildInput::FromMessagePack(const msgpack_object *msg)
{
    // msg is third arg of the MODBUILD command. It depends on the model
    // implementation to decide what do to if it's not there.
    if (msg == NULL) {
        return -1;
    }

    if (msg->type != MSGPACK_OBJECT_MAP) {
        return -1;
    }

    // 3 key/value items in the map:
    // dataset_name, features, response
    if (msg->via.map.size != 3) {
        return -1;
    }

    for (uint32_t i = 0; i < msg->via.map.size; ++i) {
        if (msg->via.map.ptr[i].key.type != MSGPACK_OBJECT_RAW) {
            return -1;
        }

        const char *key = msg->via.map.ptr[i].key.via.raw.ptr;
        auto key_size = msg->via.map.ptr[i].key.via.raw.size;
        if (strncmp(key, "dataset_name", key_size) == 0) {
            if (msg->via.map.ptr[i].val.type != MSGPACK_OBJECT_RAW) {
                return -1;
            }
            dataset_name = std::string(msg->via.map.ptr[i].val.via.raw.ptr,
                                       msg->via.map.ptr[i].val.via.raw.size);
        } else if (strncmp(key, "features", key_size) == 0) {
            if (msg->via.map.ptr[i].val.type != MSGPACK_OBJECT_ARRAY) {
                return -1;
            }

            for (uint32_t j = 0; j < msg->via.map.ptr[i].val.via.array.size; ++j) {
                if (msg->via.map.ptr[i].val.via.array.ptr[j].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
                    return -1;
                }

                feature_columns.push_back(msg->via.map.ptr[i].val.via.array.ptr[j].via.u64);
            }
        } else if (strncmp(key, "response", key_size) == 0) {
            if (msg->via.map.ptr[i].val.type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
                return -1;
            }

            response_column = msg->via.map.ptr[i].val.via.u64;
        } else {
            return -1;
        }
    }

    return 0;
}


int LinearRegression::PredictionInput::FromMessagePack(const msgpack_object *msg)
{
    if (msg == NULL) {
        return -1;
    }

    if (msg->type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    for (uint32_t i = 0; i < msg->via.array.size; ++i) {
        switch (msg->via.array.ptr[i].type) {
            case MSGPACK_OBJECT_POSITIVE_INTEGER:
                data.push_back(static_cast<double>(msg->via.array.ptr[i].via.u64));
                break;
            case MSGPACK_OBJECT_NEGATIVE_INTEGER:
                data.push_back(static_cast<double>(msg->via.array.ptr[i].via.i64));
                break;
            case MSGPACK_OBJECT_DOUBLE:
                data.push_back(msg->via.array.ptr[i].via.dec);
                break;
            default:
                return -1;
                break;
        }
    }

    return 0;
}


msgpack_sbuffer *LinearRegression::PredictionResult::ToMessagePack()
{
    msgpack_sbuffer *buffer = msgpack_sbuffer_new();
    msgpack_packer *pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_map(pk, 1);
    msgpack_pack_raw(pk, 9);
    msgpack_pack_raw_body(pk, "responses", 9);
    msgpack_pack_array(pk, predictions.size());

    for (uint32_t i = 0; i < predictions.size(); ++i) {
        msgpack_pack_double(pk, predictions[i]);
    }

    msgpack_packer_free(pk);
    return buffer;
}


}; // namespace model
}; // namespace ml
}; // namespace chimp

#endif /* end of include guard */
