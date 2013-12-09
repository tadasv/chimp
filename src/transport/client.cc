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
#include <stdlib.h>
#include "core/logging.h"
#include "transport/client.h"

namespace chimp {
namespace transport {


typedef struct ch_write_req_t {
    uv_write_t req;
    uv_buf_t buf;
    chimp::transport::Client *client;
    msgpack_sbuffer *sbuffer;
} ch_write_req_t;


static void _write_cb(uv_write_t *req, int status)
{
    ch_write_req_t *r = (ch_write_req_t*)req;
    msgpack_sbuffer_free(r->sbuffer);
    free(req);
}


Client::Client(Server *server)
{
    this->server = server;
}


int Client::Init()
{
    int r;

    r = uv_tcp_init(this->server->handle.loop, &this->handle);
    if (r) {
        CH_LOG_ERROR("uv_tcp_init: " << uv_strerror((uv_last_error(this->server->loop))));
        return r;
    }

    r = uv_accept((uv_stream_t*)&server->handle, (uv_stream_t*)&this->handle);
    if (r) {
        CH_LOG_ERROR("uv_accept: " << uv_strerror((uv_last_error(server->loop))));
        return r;
    }

    this->handle.data = this;

    return 0;
}


void Client::Write(const std::shared_ptr<AbstractResponse> &response)
{
    msgpack_sbuffer *sbuffer = response->ToMessagePack();

    ch_write_req_t *req = (ch_write_req_t*)malloc(sizeof(ch_write_req_t));
    req->client = this;
    req->buf.base = sbuffer->data;
    req->buf.len = sbuffer->size;
    req->sbuffer = sbuffer;

    uv_write((uv_write_t*)req,
             (uv_stream_t*)&this->handle,
             &req->buf, 1, _write_cb);
}


Client::~Client()
{
}

}
}
