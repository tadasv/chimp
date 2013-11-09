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
#ifndef CH_INCLUDE_GUARD_CEC85A6A_601A_4964_9263_8B2D87B43FDA
#define CH_INCLUDE_GUARD_CEC85A6A_601A_4964_9263_8B2D87B43FDA


#include <uv.h>
#include <http_parser.h>
#include <csv_parser.h>
#include <ch_hash_table.h>


typedef struct ch_http_server_settings_ {
    int port;
    int socket_backlog;
} ch_http_server_settings_t;


typedef struct ch_http_server_ {
    uv_loop_t *loop;
    uv_tcp_t handle;
    http_parser_settings parser_settings;
    csv_parser_settings_t csv_parser_settings;
    ch_http_server_settings_t *settings;
    ch_hash_table_multi_t handlers;
} ch_http_server_t;


int ch_http_server_init(ch_http_server_t *server, ch_http_server_settings_t *settings, uv_loop_t *loop);
int ch_http_server_add_handler(ch_http_server_t *server, const char *path, void *handler);
int ch_http_server_start(ch_http_server_t *server);


#endif /* end of include guard */
