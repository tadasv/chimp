/**
 * Copyright (C) Tadas Vilkeliskis
 */
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
}
