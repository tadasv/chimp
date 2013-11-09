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
#include <ch_http_message.h>


void ch_http_message_init(ch_http_message_t *msg)
{
    memset(msg, 0, sizeof(ch_http_message_t));
    msg->read_state = CH_HTTP_MESSAGE_NOT_STARTED;
    msg->method = CH_HTTP_REQUEST_METHOD_UNKNOWN;
    ch_list_init_empty(&msg->headers);
    ch_str_init_empty(&msg->path);
    ch_str_init_empty(&msg->uri);
    ch_str_init(&msg->body, "");
}


void ch_http_message_free(ch_http_message_t *msg)
{
    ch_node_t *node = msg->headers.head;
    while (node) {
        ch_keyval_t *keyval = node->data;
        ch_str_free(&keyval->key);
        ch_str_free(&keyval->value);
        free(keyval);
        node = node->next;
    }

    ch_list_free(&msg->headers);
    ch_str_free(&msg->body);
    ch_str_free(&msg->path);
    ch_str_free(&msg->uri);

    if (msg->csv_table) {
        ch_table_free(msg->csv_table);
        free(msg->csv_table);
        msg->csv_table = NULL;
    }
}


const char *ch_http_message_get_header(ch_http_message_t *msg, const char *name)
{
    assert(msg);

    ch_keyval_t *kv;
    ch_node_t *node = msg->headers.head;
    while (node) {
        kv = node->data;
        if (strcmp(name, kv->key.data) == 0) {
            return kv->value.data;
        }

        node = node->next;
    }

    return NULL;
}
