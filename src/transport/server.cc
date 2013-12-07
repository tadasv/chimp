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
#include <iostream>
#include <cassert>
#include <msgpack.h>
#include <stdlib.h>
#include <ch_log.h>
#include "transport/server.h"
#include "transport/client.h"
#include <ch_protocol.h>
#include "transport/command/ping.h"


namespace chimp {
namespace transport {


void _close_cb(uv_handle_t *client_handle)
{
    chimp::transport::Client *client = static_cast<chimp::transport::Client*>(client_handle->data);
    delete client;
}


static uv_buf_t _alloc_cb(uv_handle_t* client_handle, size_t suggested_size)
{
    uv_buf_t buf;

    CH_LOG_DEBUG("alloc_cb: allocating %d bytes", suggested_size);
    buf.base = (char*)malloc(suggested_size);
    buf.len = suggested_size;

    return buf;
}


static void _read_cb(uv_stream_t *client_handle, ssize_t nread, uv_buf_t buf)
{
    chimp::transport::Client *client = static_cast<chimp::transport::Client*>(client_handle->data);

    if (nread >= 0) {
        msgpack_unpacked msg;
        msgpack_unpacked_init(&msg);

        if (!msgpack_unpack_next(&msg, buf.base, buf.len, NULL)) {
            CH_LOG_ERROR("read: failed to unpack message");
            client->Write(CH_RESPONSE_CODE_USER_ERROR, "invalid message");
        } else if (msg.data.type != MSGPACK_OBJECT_ARRAY ||
            msg.data.via.array.size < 1 ||
            msg.data.via.array.ptr[0].type != MSGPACK_OBJECT_RAW) {
            client->Write(CH_RESPONSE_CODE_USER_ERROR, "invalid message");
        } else {
            std::string command_name(msg.data.via.array.ptr[0].via.raw.ptr,
                                     msg.data.via.array.ptr[0].via.raw.size);
            std::map<std::string, ch_command_t>::iterator iter = client->server->commands.find(command_name);
            if (iter == client->server->commands.end()) {
                client->Write(CH_RESPONSE_CODE_SERVER_ERROR, "unsupported command");
            } else {
                ch_command_t command = iter->second;
                chimp::transport::command::AbstractCommand *cmd = NULL;

                switch (command) {
                    case CH_COMMAND_PING:
                        cmd = new chimp::transport::command::Ping(client);
                        break;
                    default:
                        client->Write(CH_RESPONSE_CODE_SERVER_ERROR, "command not implemented");
                        break;
                }

                if (cmd) {
                    if (cmd->FromMessagePack(&msg) == 0) {
                        cmd->Execute();
                    } else {
                        CH_LOG_ERROR("failed to unpack message");
                    }
                    delete cmd;
                }
            }
            msgpack_unpacked_destroy(&msg);
        }
    } else {
        if (nread != UV_EOF) {
            CH_LOG_ERROR("read: %s", uv_strerror(uv_last_error(client_handle->loop)));
        }
        uv_close((uv_handle_t*)client_handle, _close_cb);
    }

    free(buf.base);
}


static void _connection_cb(uv_stream_t *server_handle, int status)
{
    chimp::transport::Server *server = static_cast<chimp::transport::Server*>(server_handle->data);
    chimp::transport::Client *client = new chimp::transport::Client(server);

    if (client->Init() != 0) {
        delete client;
        return;
    }

    CH_LOG_INFO("new connection");

    uv_read_start((uv_stream_t*)&client->handle, _alloc_cb, _read_cb);
}


Server::Server(ch_server_settings_t *settings, uv_loop_t *loop)
{
    this->loop = loop;
    this->settings = settings;
    this->commands["PING"] = CH_COMMAND_PING;
    this->commands["DSNEW"] = CH_COMMAND_DSNEW;
    uv_tcp_init(this->loop, &this->handle);
    this->handle.data = this;
}


int Server::Start()
{
    int r;

    r = uv_tcp_bind(&this->handle, uv_ip4_addr("0.0.0.0", this->settings->port));
    if (r) {
        CH_LOG_ERROR("bind: %s", uv_strerror(uv_last_error(this->loop)));
        return -1;
    }

    r = uv_listen((uv_stream_t*)&this->handle,
                  this->settings->socket_backlog,
                  _connection_cb);
    if (r) {
        CH_LOG_ERROR("listen: %s", uv_strerror(uv_last_error(this->loop)));
        return -1;
    }

    CH_LOG_INFO("starting server on 0.0.0.0:%d", this->settings->port);

    return 0;

}

}
}
