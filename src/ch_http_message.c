/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <stdlib.h>
#include <string.h>
#include <ch_http_message.h>


void ch_http_message_init(ch_http_message_t *msg)
{
    memset(msg, 0, sizeof(ch_http_message_t));
    msg->state = CH_HTTP_MESSAGE_NOT_STARTED;
    msg->method = CH_HTTP_REQUEST_METHOD_UNKNOWN;
}


void ch_http_message_free(ch_http_message_t *msg)
{
    if (msg->path.data) {
        free(msg->path.data);
        msg->path.data = NULL;
        msg->path.len = 0;
    }
}
