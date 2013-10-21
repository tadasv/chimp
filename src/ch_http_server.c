/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <assert.h>
#include <stdlib.h>
#include <ch_log.h>
#include <ch_http_server.h>
#include <ch_http_client.h>
#include <ch_http_message.h>


typedef struct ch_write_req_ {
    ch_http_client_t *client;
    uv_buf_t buf;
} ch_write_req_t;


void close_cb(uv_handle_t *client_handle);
static void _connection_cb(uv_stream_t *server_handle, int status);
static void _read_cb(uv_stream_t *client_handle, ssize_t nread, uv_buf_t buf);
static void _write_cb(uv_write_t *req, int status);
static uv_buf_t _alloc_cb(uv_handle_t* handle, size_t suggested_size);

// http
static int _message_begin(http_parser *parser);
static int _url_cb(http_parser *parser, const char *at, size_t length);
static int _header_field(http_parser *parser, const char *at, size_t len);
static int _header_value(http_parser *parser, const char *at, size_t len);
static int _headers_complete_cb(http_parser *parser);
static int _body_cb(http_parser *parser, const char *at, size_t length);
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
    client->request.read_state = CH_HTTP_MESSAGE_PROCESSING;
    return 0;
}


static int _url_cb(http_parser *parser, const char *at, size_t length)
{
    ch_http_client_t *client = parser->data;
    struct http_parser_url parser_url;

    ch_str_linit(&client->request.uri, at, length);

    if (http_parser_parse_url(client->request.uri.data,
                              client->request.uri.len,
                              0, &parser_url) != 0) {
        client->request.read_state = CH_HTTP_MESSAGE_ERROR_INVALID;
        return -1;
    }

    if (_http_parser_valid_field(&parser_url, UF_PATH)) {
        uint16_t offset, len;
        _http_parser_field_data(&parser_url, UF_PATH, &offset, &len);
        ch_str_linit(&client->request.path,
                     client->request.uri.data + offset,
                     len);
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


static int _header_field(http_parser *parser, const char *at, size_t len)
{
    ch_http_client_t *client = parser->data;

    if (client->header_value.len != 0) {
        ch_keyval_t *header = (ch_keyval_t*)malloc(sizeof(ch_keyval_t));
        ch_str_init(&header->key, client->header_field.data);
        ch_str_init(&header->value, client->header_value.data);
        ch_list_append(&client->request.headers, header);

        client->header_value.len = 0;
        client->header_field.len = 0;
    }

    ch_str_lcat(&client->header_field, at, len);
    return 0;
}


static int _header_value(http_parser *parser, const char *at, size_t len)
{
    ch_http_client_t *client = parser->data;
    ch_str_lcat(&client->header_value, at, len);
    return 0;
}


static int _headers_complete_cb(http_parser *parser)
{
    ch_http_client_t *client = parser->data;

    ch_node_t *node = client->request.headers.head;
    while (node) {
        ch_keyval_t *kv = node->data;
        node = node->next;
    }

    return 0;
}


void http_not_found_handler(ch_http_client_t *client)
{
    ch_str_t str;
    ch_str_init(&str, "HTTP/1.0 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 3\r\n\r\n404");
    // Write will take over memory ownership.
    ch_http_client_finish(client, &str);
}


static int _body_cb(http_parser *parser, const char *at, size_t length)
{
    ch_http_client_t *client = parser->data;
    // TODO body limits
    ch_str_lcat(&client->request.body, at, length);
    return 0;
}


static int _message_complete(http_parser *parser)
{
    ch_http_client_t *client = parser->data;
    client->request.read_state = CH_HTTP_MESSAGE_FINISHED;
#if 0
    uv_work_t *request_job = malloc(sizeof(uv_work_t));

    // handle request in the background
    request_job->data = client;
    // user loop from the server since client can
    // go away at any point in time.
    uv_queue_work(client->server->handle.loop,
                  request_job,
                  _handle_request_cb,
                  _after_handle_request_cb);
#endif

    ch_http_handler_t handler;
    ch_hash_element_t *elem;

    CH_LOG_DEBUG("method: %d, body: %s", client->request.method, client->request.body.data);

    /* call request handler */
    elem = ch_hash_table_find(&client->server->handlers,
                              client->request.path.data,
                              client->request.path.len);
    if (!elem) {
        http_not_found_handler(client);
        return 0;
    }

    handler = (ch_http_handler_t)elem->data;
    handler(client);

    return 0;
}


uv_buf_t _alloc_cb(uv_handle_t* handle, size_t suggested_size)
{
    uv_buf_t buf;
    buf.base = malloc(suggested_size);
    buf.len = suggested_size;
    return buf;
}


void close_cb(uv_handle_t *client_handle)
{
    ch_http_client_t *client = client_handle->data;
    ch_http_client_free(client);
    free(client);
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

        if (client->request.read_state != CH_HTTP_MESSAGE_FINISHED) {
            if (parsed != nread) {
                client->request.read_state = CH_HTTP_MESSAGE_ERROR_INVALID;
            }
        }
    } else {
        if (nread != UV_EOF) {
            CH_LOG_ERROR("read: %s", uv_strerror(uv_last_error(client_handle->loop)));
        }
        uv_close((uv_handle_t*)client_handle, close_cb);
    }

    free(buf.base);
}


static void _connection_cb(uv_stream_t *server_handle, int status)
{
    int r;
    ch_http_server_t *server = server_handle->data;
    ch_http_client_t *client = (ch_http_client_t*)malloc(sizeof(ch_http_client_t));

    if (ch_http_client_init(client, server)) {
        free(client);
        return;
    }

    uv_read_start((uv_stream_t*)&client->handle, _alloc_cb, _read_cb);
}


int ch_http_server_init(ch_http_server_t *server, ch_http_server_settings_t *settings, uv_loop_t *loop)
{
    assert(server);
    assert(loop);

    server->loop = loop;
    server->settings = settings;

    memset(&server->parser_settings, 0, sizeof(server->parser_settings));
    server->parser_settings.on_message_begin = _message_begin;
    server->parser_settings.on_url = _url_cb;
    server->parser_settings.on_status_complete = NULL;
    server->parser_settings.on_header_field = _header_field;
    server->parser_settings.on_header_value = _header_value;
    server->parser_settings.on_headers_complete = _headers_complete_cb;
    server->parser_settings.on_body = _body_cb;
    server->parser_settings.on_message_complete = _message_complete;

    uv_tcp_init(server->loop, &server->handle);
    server->handle.data = server;

    // TODO switch to hash table that can handle collisions
    ch_hash_table_init(&server->handlers, 64);

    return 0;
}


int ch_http_server_add_handler(ch_http_server_t *server, const char *path, void *handler)
{
    assert(server);

    int len = strlen(path);

    if (ch_hash_table_find(&server->handlers, (void*)path, len)) {
        return -1;
    }

    ch_hash_table_add(&server->handlers, (void*)path, len, handler);
    return 0;
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
