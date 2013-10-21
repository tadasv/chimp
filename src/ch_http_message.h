/**
 * Copyright (C) Tadas Vilkeliskis
 */
#ifndef INCLUDE_GUARD_44ED6BBE_9564_448B_854A_287ED562DD43
#define INCLUDE_GUARD_44ED6BBE_9564_448B_854A_287ED562DD43


#include <ch_string.h>


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
    ch_http_message_state_t state;
    ch_http_request_method_t method;
    ch_str_t path;
} ch_http_message_t;


void ch_http_message_init(ch_http_message_t *msg);
void ch_http_message_free(ch_http_message_t *msg);


#endif /* end of include guard */
