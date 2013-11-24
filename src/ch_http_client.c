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
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ch_log.h>
#include <ch_http_client.h>


const char *HTTP_STATUS_LINE_200 = "HTTP/1.0 200 OK\r\n";
const char *HTTP_STATUS_LINE_400 = "HTTP/1.0 400 Bad Request\r\n";
const char *HTTP_STATUS_LINE_404 = "HTTP/1.0 404 Not Found\r\n";
const char *HTTP_STATUS_LINE_500 = "HTTP/1.0 500 Internal Error\r\n";


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

    http_parser_init(&client->parser, HTTP_REQUEST);
    csv_parser_init(&client->csv_parser);

    client->server = server;
    client->handle.data = client;
    client->parser.data = client;
    client->csv_parser.data = client;
    client->parser_settings = &server->parser_settings;
    client->csv_parser_settings = &server->csv_parser_settings;
    /* initialize temporary buffers for parsing http headers */
    ch_str_init_alloc(&client->header_field, 128);
    ch_str_init_alloc(&client->header_value, 128);
    ch_str_init_alloc(&client->csv_field, 16);
    client->current_csv_row = -1;
    client->current_csv_col = -1;

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
    assert(client);
    ch_http_write_req_t *req;

    if (!str) {
        /* don't do anything just close the connection */
        uv_close((uv_handle_t*)&client->handle, close_cb);
        return;
    }

    req = malloc(sizeof(ch_http_write_req_t));
    req->client = client;
    req->buf.base = str->data;
    req->buf.len = str->len;
    req->close = 1;

    uv_write((uv_write_t*)req,
             (uv_stream_t*)&client->handle,
             &req->buf, 1, _write_cb);
}


void ch_http_client_write_header(ch_http_client_t *client, const char *name, const char *value, int final_header)
{
    assert(client);
    assert(name);
    assert(value);

    ch_str_t header_str;
    int name_len = strlen(name);
    int value_len = strlen(value);
    /* +3 = \r\n\0 */
    int buffer_size = name_len + value_len + 3;

    if (final_header) {
        buffer_size += 2;
    }

    ch_str_init_alloc(&header_str, buffer_size);
    ch_str_lcat(&header_str, name, name_len);
    ch_str_lcat(&header_str, ": ", 2);
    ch_str_lcat(&header_str, value, value_len);

    if (final_header) {
        ch_str_lcat(&header_str, "\r\n\r\n", 4);
    } else {
        ch_str_lcat(&header_str, "\r\n", 2);
    }

    ch_http_client_write(client, &header_str);
}


void ch_http_client_write_status_line(ch_http_client_t *client, int status_code)
{
    ch_str_t status_line;

    switch (status_code) {
        case 200:
            ch_str_init(&status_line, HTTP_STATUS_LINE_200);
            break;
        case 400:
            ch_str_init(&status_line, HTTP_STATUS_LINE_400);
            break;
        case 404:
            ch_str_init(&status_line, HTTP_STATUS_LINE_404);
            break;
        case 500:
        default:
            ch_str_init(&status_line, HTTP_STATUS_LINE_500);
            break;
    }

    ch_http_client_write(client, &status_line);
}


void ch_http_client_free(ch_http_client_t *client)
{
    ch_http_message_free(&client->request);
    ch_str_free(&client->header_field);
    ch_str_free(&client->header_value);
    ch_str_free(&client->csv_field);
}
