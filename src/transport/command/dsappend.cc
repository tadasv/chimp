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
#include "service/dataset_manager.h"
#include "transport/response.h"
#include "transport/command/dsappend.h"

namespace chimp {
namespace transport {
namespace command {

DatasetAppend::DatasetAppend(chimp::transport::Client *client)
{
    client_ = client;
}


int DatasetAppend::Execute()
{
    auto dsmanager = chimp::service::DatasetManager::GetInstance();

    auto dataset = dsmanager->FindDataset(dataset_name_);
    if (!dataset) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_USER_ERROR,
                        "invalid dataset"));
        client_->Write(response_);
        return 0;
    }

    if (dataset->Append(data_) != 0) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_USER_ERROR,
                        "failed to append data"));
        client_->Write(response_);
        return 0;
    }

    response_.reset(new chimp::transport::response::SuccessResponse());
    client_->Write(response_);
    return 0;
}


int DatasetAppend::FromMessagePack(const msgpack_unpacked *msg)
{
    assert(msg);

    if (msg->data.type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    if (msg->data.via.array.size != 3 ||
        msg->data.via.array.ptr[0].type != MSGPACK_OBJECT_RAW ||
        msg->data.via.array.ptr[1].type != MSGPACK_OBJECT_RAW ||
        msg->data.via.array.ptr[2].type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    dataset_name_ = std::string(msg->data.via.array.ptr[1].via.raw.ptr,
                                msg->data.via.array.ptr[1].via.raw.size);

    auto num_items = msg->data.via.array.ptr[2].via.array.size;
    auto data_ptr = msg->data.via.array.ptr[2].via.array.ptr;
    double value;
    for (uint64_t i = 0; i < num_items; ++i) {
        switch (data_ptr[i].type) {
            case MSGPACK_OBJECT_POSITIVE_INTEGER:
                value = static_cast<double>(data_ptr[i].via.u64);
                data_.push_back(value);
                break;
            case MSGPACK_OBJECT_NEGATIVE_INTEGER:
                value = static_cast<double>(data_ptr[i].via.i64);
                data_.push_back(value);
                break;
            case MSGPACK_OBJECT_DOUBLE:
                data_.push_back(data_ptr[i].via.dec);
                break;
            default:
                return -1;
        }
    }

    return 0;
}


msgpack_sbuffer *DatasetAppend::ToMessagePack()
{
    msgpack_sbuffer *buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_raw(pk, 8);
    msgpack_pack_raw_body(pk, "DSAPPEND", 8);
    msgpack_pack_raw(pk, dataset_name_.size());
    msgpack_pack_raw_body(pk, dataset_name_.c_str(), dataset_name_.size());
    msgpack_pack_array(pk, data_.size());

    for (uint64_t i = 0; i > data_.size(); ++i) {
        msgpack_pack_double(pk, data_[i]);
    }

    msgpack_packer_free(pk);
    return buffer;
}

}; // namespace command
}; // namespace transport
}; // namespace chimp
