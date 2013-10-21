/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <stdlib.h>
#include <ch_log.h>
#include <ch_http_client.h>


typedef struct ch_http_write_req_ {
    uv_write_t req;
    uv_buf_t buf;
    ch_http_client_t *client;
    int close;
} ch_http_write_req_t;


extern void close_cb(uv_handle_t *client_handle);


int ch_http_client_init(ch_http_client_t *client, ch_http_server_t *server)
{
    int r;

    ch_http_message_init(&client->request);

    r = uv_tcp_init(server->handle.loop, &client->handle);
    if (r) {
        CH_LOG_ERROR("uv_tcp_init: %s", uv_strerror((uv_last_error(server->loop))));
        return r;
    }

    r = uv_accept((uv_stream_t*)&server->handle, (uv_stream_t*)&client->handle);
    if (r) {
        CH_LOG_ERROR("uv_accept: %s", uv_strerror((uv_last_error(server->loop))));
        return r;
    }

    client->server = server;
    client->handle.data = client;
    client->parser.data = client;
    client->parser_settings = &server->parser_settings;
    /* initialize temporary buffers for parsing http headers */
    ch_str_init_alloc(&client->header_field, 128);
    ch_str_init_alloc(&client->header_value, 128);

    http_parser_init(&client->parser, HTTP_REQUEST);

    return 0;
}


static void _write_cb(uv_write_t *req, int status)
{
    ch_http_write_req_t *r = (ch_http_write_req_t*)req;
    free(r->buf.base);

    if (r->close) {
        uv_close((uv_handle_t*)&r->client->handle, close_cb);
    }

    free(req);
}


void ch_http_client_write(ch_http_client_t *client, ch_str_t *str)
{
    ch_http_write_req_t *req = malloc(sizeof(ch_http_write_req_t));
    req->client = client;
    req->buf.base = str->data;
    req->buf.len = str->len;
    req->close = 0;

    uv_write((uv_write_t*)req,
             (uv_stream_t*)&client->handle,
             &req->buf, 1, _write_cb);
}


void ch_http_client_finish(ch_http_client_t *client, ch_str_t *str)
{
    ch_http_write_req_t *req = malloc(sizeof(ch_http_write_req_t));
    req->client = client;
    req->buf.base = str->data;
    req->buf.len = str->len;
    req->close = 1;

    uv_write((uv_write_t*)req,
             (uv_stream_t*)&client->handle,
             &req->buf, 1, _write_cb);
}


void ch_http_client_free(ch_http_client_t *client)
{
    ch_http_message_free(&client->request);
    ch_str_free(&client->header_field);
    ch_str_free(&client->header_value);
}
