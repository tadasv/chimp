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
#include <cassert>
#include <algorithm>
#include "transport/command/modpredict.h"
#include "transport/response.h"
#include "service/model_manager.h"
#include "ml/linear_regression.h"

namespace chimp {
namespace transport {
namespace command {


ModelPredict::ModelPredict(chimp::transport::Client *client)
{
    client_ = client;
}


int ModelPredict::Execute()
{
    auto model_manager = chimp::service::ModelManager::GetInstance();

    auto model = model_manager->FindModel(model_name_);
    if (!model) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_USER_ERROR,
                        "invalid model"));
        client_->Write(response_);
        return 0;
    }

    auto prediction = model->Predict(model_input_.get());
    auto data = prediction->ToMessagePack();

    auto response = new Response();
    response_.reset(response);
    response->SetResponse(data);
    msgpack_sbuffer_free(data);
    client_->Write(response_);

    return 0;
}


int ModelPredict::FromMessagePack(const msgpack_unpacked *msg)
{
    assert(msg);

    if (msg->data.type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    if (msg->data.via.array.size <= 2 ||
        msg->data.via.array.ptr[0].type != MSGPACK_OBJECT_RAW ||
        msg->data.via.array.ptr[1].type != MSGPACK_OBJECT_RAW) {
        return -1;
    }

    model_name_ = std::string(msg->data.via.array.ptr[1].via.raw.ptr,
                              msg->data.via.array.ptr[1].via.raw.size);
    // TODO unpack model input
    // this is currently hard coded to linear model only since
    // that's the only thing there is now.
    model_input_.reset(new chimp::ml::model::LinearRegression::PredictionInput());
    if (msg->data.via.array.size > 2) {
        return model_input_->FromMessagePack(&msg->data.via.array.ptr[2]);
    }

    return model_input_->FromMessagePack(NULL);
}


msgpack_sbuffer *ModelPredict::ToMessagePack()
{
    return NULL;
}



ModelPredict::Response::Response()
{
}


void ModelPredict::Response::SetResponse(const msgpack_sbuffer *data)
{
    serialized_data_.reset(new char[data->size]);
    memcpy(serialized_data_.get(), data->data, data->size);
    data_size_ = data->size;
}


msgpack_sbuffer *ModelPredict::Response::ToMessagePack()
{
    msgpack_sbuffer *buffer = msgpack_sbuffer_new();
    msgpack_packer *pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_unsigned_int(pk, chimp::transport::response::ResponseCode::RESPONSE_CODE_OK);
    pk->callback(pk->data, serialized_data_.get(), data_size_);
    //msgpack_pack_append_buffer(pk, serialized_data_.get(), data_size_);

    // error
    msgpack_pack_nil(pk);

    msgpack_packer_free(pk);
    return buffer;
}


}; // namespace command
}; // namespace transport
}; // namespace chimp
