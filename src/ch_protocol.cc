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
#include <ch_protocol.h>

ch_response_message_t::ch_response_message_t()
{
    this->code = CH_RESPONSE_CODE_OK;
}


ch_response_message_t::ch_response_message_t(ch_response_code_t code)
{
    this->code = code;
}


msgpack_sbuffer *ch_response_message_t::serialize()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    int array_len = this->code < CH_RESPONSE_CODE_USER_ERROR ? 1 : 2;

    msgpack_pack_array(pk, array_len);
    msgpack_pack_unsigned_int(pk, this->code);

    if (array_len > 1) {
        msgpack_pack_raw(pk, this->error.size());
        msgpack_pack_raw_body(pk, this->error.c_str(), this->error.size());
    }

    msgpack_packer_free(pk);
    // caller is responsible for freeing the buffer
    return buffer;
}


int ch_response_message_t::unserialize(const msgpack_unpacked *msg)
{
    return -1;
}


msgpack_sbuffer *ch_message_dsnew_t::serialize()
{
    msgpack_sbuffer* buffer = msgpack_sbuffer_new();
    msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

    msgpack_pack_array(pk, 2);
    msgpack_pack_raw(pk, 5);
    msgpack_pack_raw_body(pk, "DSNEW", 5);
    msgpack_pack_array(pk, 2);
    msgpack_pack_raw(pk, this->name.size());
    msgpack_pack_raw_body(pk, this->name.c_str(), this->name.size());
    msgpack_pack_unsigned_int(pk, this->num_columns);

    msgpack_packer_free(pk);
    return buffer;
}


int ch_message_dsnew_t::unserialize(const msgpack_unpacked *msg)
{
    assert(msg);

    if (msg->data.type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    if (msg->data.via.array.size != 2) {
        return -1;
    }

    if (msg->data.via.array.ptr[0].type != MSGPACK_OBJECT_RAW) {
        return -1;
    }

    if (msg->data.via.array.ptr[1].type != MSGPACK_OBJECT_ARRAY) {
        return -1;
    }

    if (msg->data.via.array.ptr[1].via.array.size != 2) {
        return -1;
    }

    if (msg->data.via.array.ptr[1].via.array.ptr[0].type != MSGPACK_OBJECT_RAW) {
        return -1;
    }

    if (msg->data.via.array.ptr[1].via.array.ptr[1].type != MSGPACK_OBJECT_POSITIVE_INTEGER) {
        return -1;
    }

    this->num_columns = msg->data.via.array.ptr[1].via.array.ptr[1].via.u64;
    this->name = std::string(msg->data.via.array.ptr[1].via.array.ptr[0].via.raw.ptr,
                             msg->data.via.array.ptr[1].via.array.ptr[0].via.raw.size);

    return 0;
}
