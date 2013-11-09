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
#ifndef INCLUDE_GUARD_44ED6BBE_9564_448B_854A_287ED562DD43
#define INCLUDE_GUARD_44ED6BBE_9564_448B_854A_287ED562DD43


#include <ch_string.h>
#include <ch_list.h>
#include <ch_table.h>


typedef enum ch_http_message_state_ {
    CH_HTTP_MESSAGE_NOT_STARTED = 0,
    CH_HTTP_MESSAGE_PROCESSING,
    CH_HTTP_MESSAGE_FINISHED,
    CH_HTTP_MESSAGE_ERROR_INVALID
} ch_http_message_state_t;


typedef enum ch_http_request_method_ {
    CH_HTTP_REQUEST_METHOD_GET = 0,
    CH_HTTP_REQUEST_METHOD_POST,
    CH_HTTP_REQUEST_METHOD_PUT,
    CH_HTTP_REQUEST_METHOD_DELETE,
    CH_HTTP_REQUEST_METHOD_UNKNOWN,
} ch_http_request_method_t;


typedef struct ch_http_message_ {
    ch_http_message_state_t read_state;
    ch_http_request_method_t method;
    ch_str_t uri;
    ch_str_t path;
    ch_str_t body;
    ch_list_t headers;
    ch_table_t *csv_table;
} ch_http_message_t;


void ch_http_message_init(ch_http_message_t *msg);
void ch_http_message_free(ch_http_message_t *msg);

const char *ch_http_message_get_header(ch_http_message_t *msg, const char *name);

#endif /* end of include guard */
