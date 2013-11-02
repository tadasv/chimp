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
#ifndef INCLUDE_GUARD_55AC1358_C4D3_4903_9B49_E1731F80619B
#define INCLUDE_GUARD_55AC1358_C4D3_4903_9B49_E1731F80619B

#include <uv.h>
#include <http_parser.h>
#include <csv_parser.h>
#include <ch_http_message.h>
#include <ch_http_server.h>


typedef struct ch_http_client_ {
    uv_tcp_t handle;
    http_parser parser;
    http_parser_settings *parser_settings;
    csv_parser_t csv_parser;
    csv_parser_settings_t *csv_parser_settings;
    ch_http_server_t *server;
    ch_http_message_t request;
    /* temp buffers for parsing http headers */
    ch_str_t header_field;
    ch_str_t header_value;
    /* temp variables for building csv field */
    ch_str_t csv_field;
    int current_csv_row;
    int current_csv_col;
} ch_http_client_t;


typedef void(*ch_http_handler_t)(ch_http_client_t *client);


int ch_http_client_init(ch_http_client_t *client, ch_http_server_t *server);
void ch_http_client_write(ch_http_client_t *client, ch_str_t *str);
void ch_http_client_finish(ch_http_client_t *client, ch_str_t *str);
void ch_http_client_free(ch_http_client_t *client);



#endif /* end of include guard */
