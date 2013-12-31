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
#include "transport/command/modbuild.h"
#include "transport/response.h"
#include "ml/linear_regression.h"
#include "service/model_manager.h"

namespace chimp {
namespace transport {
namespace command {


ModelBuild::ModelBuild(chimp::transport::Client *client)
{
    client_ = client;
}


int ModelBuild::Execute()
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

    // TODO build in background
    if (model->Build(model_input_.get()) != 0) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_USER_ERROR,
                        "build failed"));
        client_->Write(response_);
        return 0;
    }

    response_.reset(new chimp::transport::response::SuccessResponse());
    client_->Write(response_);
    return 0;
}


int ModelBuild::FromMessagePack(const msgpack_unpacked *msg)
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
    model_input_.reset(new chimp::ml::model::LinearRegression::BuildInput());
    if (msg->data.via.array.size > 2) {
        return model_input_->FromMessagePack(&msg->data.via.array.ptr[2]);
    }

    return model_input_->FromMessagePack(NULL);
}


msgpack_sbuffer *ModelBuild::ToMessagePack()
{
    return NULL;
}

}; // namespace command
}; // namespace transport
}; // namespace chimp
