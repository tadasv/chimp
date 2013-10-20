/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <stdlib.h>
#include "log.h"
#include "http_server.h"


typedef struct ch_http_client_ {
    uv_tcp_t handle;
    http_parser parser;
    http_parser_settings *parser_settings;
} ch_http_client_t;


static void _close_cb(uv_handle_t *client_handle);
static void _connection_cb(uv_stream_t *server_handle, int status);
static int _headers_complete_cb(http_parser *parser);
static void _read_cb(uv_stream_t *client_handle, ssize_t nread, uv_buf_t buf);
static uv_buf_t _alloc_cb(uv_handle_t* handle, size_t suggested_size);


static int _headers_complete_cb(http_parser *parser)
{
    ch_http_client_t *client = parser->data;
    uv_close((uv_handle_t*)&client->handle, _close_cb);
    return 1;
}


uv_buf_t _alloc_cb(uv_handle_t* handle, size_t suggested_size)
{
    uv_buf_t buf;
    buf.base = malloc(suggested_size);
    buf.len = suggested_size;
    return buf;
}


static void _close_cb(uv_handle_t *client_handle)
{
    ch_http_client_t *client = client_handle->data;
    free(client);
    CH_LOG_DEBUG("closed");
}


static void _read_cb(uv_stream_t *client_handle, ssize_t nread, uv_buf_t buf)
{
    ch_http_client_t *client = client_handle->data;
    size_t parsed;

    if (nread >= 0) {
        parsed = http_parser_execute(&client->parser,
                                     client->parser_settings,
                                     buf.base,
                                     nread);
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
    int r;
    ch_http_server_t *server = server_handle->data;
    ch_http_client_t *client = (ch_http_client_t*)malloc(sizeof(ch_http_client_t));

    r = uv_tcp_init(server_handle->loop, &client->handle);
    if (r) {
        CH_LOG_ERROR("uv_tcp_init: %s", uv_strerror((uv_last_error(server->loop))));
        return;
    }

    r = uv_accept(server_handle, (uv_stream_t*)&client->handle);
    if (r) {
        CH_LOG_ERROR("uv_accept: %s", uv_strerror((uv_last_error(server->loop))));
        return;
    }

    client->handle.data = client;
    client->parser.data = client;
    client->parser_settings = &server->http_parser_settings;

    http_parser_init(&client->parser, HTTP_REQUEST);

    CH_LOG_DEBUG("connected");
    uv_read_start((uv_stream_t*)&client->handle, _alloc_cb, _read_cb);
}


int ch_http_server_init(ch_http_server_t *server, ch_http_server_settings_t *settings, uv_loop_t *loop)
{
    server->loop = loop;
    server->settings = settings;
    memset(&server->http_parser_settings, 0, sizeof(server->http_parser_settings));
    server->http_parser_settings.on_headers_complete = _headers_complete_cb;

    uv_tcp_init(server->loop, &server->handle);
    server->handle.data = server;
}


int ch_http_server_start(ch_http_server_t *server)
{
    int r;

    r = uv_tcp_bind(&server->handle, uv_ip4_addr("0.0.0.0", server->settings->port));
    if (r) {
        CH_LOG_ERROR("bind: %s", uv_strerror(uv_last_error(server->loop)));
        return -1;
    }

    r = uv_listen((uv_stream_t*)&server->handle,
                  server->settings->socket_backlog,
                  _connection_cb);
    if (r) {
        CH_LOG_ERROR("listen: %s", uv_strerror(uv_last_error(server->loop)));
        return -1;
    }

    CH_LOG_INFO("starting http server on 0.0.0.0:%d", server->settings->port);

    return 0;
}
