/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <stdlib.h>
#include "ch_log.h"
#include "ch_http_server.h"
#include "ch_http_message.h"


typedef struct ch_http_client_ {
    uv_tcp_t handle;
    http_parser parser;
    http_parser_settings *parser_settings;
    ch_http_message_t request;
} ch_http_client_t;


static void _close_cb(uv_handle_t *client_handle);
static void _connection_cb(uv_stream_t *server_handle, int status);
static void _read_cb(uv_stream_t *client_handle, ssize_t nread, uv_buf_t buf);
static uv_buf_t _alloc_cb(uv_handle_t* handle, size_t suggested_size);

// http
static int _message_begin(http_parser *parser);
static int _url_cb(http_parser *parser, const char *at, size_t length);
static int _headers_complete_cb(http_parser *parser);
static int _message_complete(http_parser *parser);


static inline int _http_parser_valid_field(const struct http_parser_url *p,
                                           enum http_parser_url_fields field)
{
    return (p->field_set & (1 << field));
}


static inline void _http_parser_field_data(const struct http_parser_url *p,
                                           enum http_parser_url_fields field,
                                           uint16_t *off,
                                           uint16_t *len)
{
    *off = p->field_data[field].off;
    *len = p->field_data[field].len;
}


static int _message_begin(http_parser *parser)
{
    ch_http_client_t *client = parser->data;
    client->request.state = CH_HTTP_MESSAGE_PROCESSING;
    return 0;
}


static int _url_cb(http_parser *parser, const char *at, size_t length)
{
    ch_http_client_t *client = parser->data;
    struct http_parser_url parser_url;

    CH_LOG_DEBUG("url cb");
    if (http_parser_parse_url(at, length, 0, &parser_url) != 0) {
        client->request.state = CH_HTTP_MESSAGE_ERROR_INVALID;
        return -1;
    }

    if (_http_parser_valid_field(&parser_url, UF_PATH)) {
        uint16_t offset, len;
        _http_parser_field_data(&parser_url, UF_PATH, &offset, &len);

        client->request.path.len = len;
        client->request.path.data = malloc(len);
        strncpy(client->request.path.data, at + offset, len);
        client->request.path.data[len] = '\0';
    }

    switch (parser->method) {
        case HTTP_GET:
            client->request.method = CH_HTTP_REQUEST_METHOD_GET;
            break;
        case HTTP_PUT:
            client->request.method = CH_HTTP_REQUEST_METHOD_PUT;
            break;
        case HTTP_POST:
            client->request.method = CH_HTTP_REQUEST_METHOD_POST;
            break;
        case HTTP_DELETE:
            client->request.method = CH_HTTP_REQUEST_METHOD_DELETE;
            break;
        default:
            break;
    }

    return 0;
}


static int _headers_complete_cb(http_parser *parser)
{
    ch_http_client_t *client = parser->data;
    CH_LOG_DEBUG("path: %s", client->request.path.data);
    uv_close((uv_handle_t*)&client->handle, _close_cb);
    return 1;
}


static int _message_complete(http_parser *parser)
{
    ch_http_client_t *client = parser->data;
    client->request.state = CH_HTTP_MESSAGE_FINISHED;
    return 0;
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
    // release request data
    ch_http_message_free(&client->request);
    // release client
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

        if (client->request.state != CH_HTTP_MESSAGE_FINISHED) {
            if (parsed != nread) {
                client->request.state = CH_HTTP_MESSAGE_ERROR_INVALID;
            }
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
    int r;
    ch_http_server_t *server = server_handle->data;
    ch_http_client_t *client = (ch_http_client_t*)malloc(sizeof(ch_http_client_t));

    ch_http_message_init(&client->request);

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
    client->parser_settings = &server->parser_settings;

    http_parser_init(&client->parser, HTTP_REQUEST);

    CH_LOG_DEBUG("connected");
    uv_read_start((uv_stream_t*)&client->handle, _alloc_cb, _read_cb);
}


int ch_http_server_init(ch_http_server_t *server, ch_http_server_settings_t *settings, uv_loop_t *loop)
{
    server->loop = loop;
    server->settings = settings;

    memset(&server->parser_settings, 0, sizeof(server->parser_settings));
    server->parser_settings.on_message_begin = _message_begin;
    server->parser_settings.on_url = _url_cb;
    server->parser_settings.on_status_complete = NULL;
    server->parser_settings.on_header_field = NULL;
    server->parser_settings.on_header_value = NULL;
    server->parser_settings.on_headers_complete = _headers_complete_cb;
    server->parser_settings.on_body = NULL;
    server->parser_settings.on_message_complete = _message_complete;

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
