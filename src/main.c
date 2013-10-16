#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>
#include "http_parser.h"


static uv_tcp_t http_server;
static http_parser_settings parser_settings;

typedef struct {
    uv_tcp_t handle;
    http_parser parser;
} http_client_t;


uv_buf_t on_alloc(uv_handle_t* handle, size_t suggested_size)
{
    uv_buf_t buf;
    buf.base = malloc(suggested_size);
    buf.len = suggested_size;
    return buf;
}


void on_close(uv_handle_t *handle)
{
    http_client_t *client = handle->data;
    free(client);
    fprintf(stdout, "closed\n");
}


int on_headers_complete(http_parser *parser)
{
    http_client_t *client = parser->data;
    uv_close((uv_handle_t*)&client->handle, on_close);
    return 1;
}


void on_read(uv_stream_t *stream, ssize_t nread, uv_buf_t buf)
{
    http_client_t *client = stream->data;
    size_t parsed;

    if (nread >= 0) {
        parsed = http_parser_execute(&client->parser,
                                     &parser_settings,
                                     buf.base,
                                     nread);
        fprintf(stdout, "%s", buf.base);
    } else {
        if (nread != UV_EOF) {
            fprintf(stderr, "read: %s\n", uv_strerror(nread));
        }
        uv_close((uv_handle_t*)stream, on_close);
    }

    free(buf.base);
}


void work_cb(uv_work_t *req)
{
    fprintf(stdout, "work callback\n");
}


void after_work_cb(uv_work_t *req, int status)
{
    fprintf(stdout, "after work callback\n");
}


void on_connected(uv_stream_t* server, int status)
{
    assert(server == (uv_stream_t*)&http_server);
    assert(status == 0);

    http_client_t *client = (http_client_t*)malloc(sizeof(http_client_t));
    int r = uv_tcp_init(server->loop, &client->handle);
    if (r) {
        fprintf(stderr, "uv_tcp_init: %s\n", uv_strerror(r));
        return;
    }

    r = uv_accept(server, (uv_stream_t*)&client->handle);
    if (r) {
        fprintf(stderr, "accept: %s\n", uv_strerror(r));
        return;
    }

    client->handle.data = client;
    client->parser.data = client;

    http_parser_init(&client->parser, HTTP_REQUEST);

    fprintf(stdout, "connected\n");

    uv_work_t *work_req = malloc(sizeof(uv_work_t));
    r = uv_queue_work(server->loop, work_req, work_cb, after_work_cb);
    if (r) {
        fprintf(stderr, "uv_queue_work: %s\n", uv_strerror(r));
    }

    uv_read_start((uv_stream_t*)&client->handle, on_alloc, on_read);
}


int main(int argc, const char *argv[])
{
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_init(loop, &http_server);
    parser_settings.on_headers_complete = on_headers_complete;

    int r = uv_tcp_bind(&http_server, uv_ip4_addr("0.0.0.0", 8000));
    if (r) {
        fprintf(stderr, "bind: %s\n", uv_strerror(r));
        return -1;
    }

    r = uv_listen((uv_stream_t*)&http_server, 128, on_connected);
    if (r) {
        fprintf(stderr, "listen: %s\n", uv_strerror(r));
        return -1;
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
