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
#include "transport/command/dsnew.h"
#include "db/dataset.h"

namespace chimp {
namespace transport {
namespace command {


DatasetNew::DatasetNew(chimp::transport::Client *client)
{
    client_ = client;
}


int DatasetNew::Execute()
{
    auto dsmanager = chimp::service::DatasetManager::GetInstance();

    if (dsmanager->DatasetExists(name_)) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_USER_ERROR,
                        "dataset exists"));
        client_->Write(response_);
        return 0;
    }

    std::shared_ptr<chimp::db::dataset::AbstractDataset> dataset(new chimp::db::dataset::Dataset(name_, num_columns_));
    if (dsmanager->AddDataset(dataset) != 0) {
        response_.reset(new chimp::transport::response::ErrorResponse(
                        chimp::transport::response::RESPONSE_CODE_SERVER_ERROR,
                        "failed to create dataset"));
        client_->Write(response_);
        return 0;
    }

    response_.reset(new chimp::transport::response::SuccessResponse());
    client_->Write(response_);
    return 0;
}


int DatasetNew::FromMessagePack(const msgpack_unpacked *msg)
{
    assert(msg);

    if (msg->data.type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    if (msg->data.via.array.size != 3 ||
        msg->data.via.array.ptr[0].type != MSGPACK_OBJECT_RAW ||
        msg->data.via.array.ptr[1].type != MSGPACK_OBJECT_RAW ||
        msg->data.via.array.ptr[2].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
        return -1;
    }

    name_ = std::string(msg->data.via.array.ptr[1].via.raw.ptr,
                        msg->data.via.array.ptr[1].via.raw.size);
    num_columns_ = msg->data.via.array.ptr[1].via.u64;

    return 0;
}


msgpack_sbuffer *DatasetNew::ToMessagePack()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 3);
    msgpack_pack_raw(pk, 5);
    msgpack_pack_raw_body(pk, "DSNEW", 5);
    msgpack_pack_raw(pk, name_.size());
    msgpack_pack_raw_body(pk, name_.c_str(), name_.size());
    msgpack_pack_unsigned_int(pk, num_columns_);

    msgpack_packer_free(pk);
    return buffer;
}


}
}
}
