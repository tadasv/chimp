/**
 * Copyright (C) Tadas Vilkeliskis
 */
#ifndef INCLUDE_GUARD_55AC1358_C4D3_4903_9B49_E1731F80619B
#define INCLUDE_GUARD_55AC1358_C4D3_4903_9B49_E1731F80619B

#include <uv.h>
#include <http_parser.h>
#include <ch_http_message.h>
#include <ch_http_server.h>


typedef struct ch_http_client_ {
    uv_tcp_t handle;
    http_parser parser;
    http_parser_settings *parser_settings;
    ch_http_server_t *server;
    ch_http_message_t request;
    /* temp buffers for parsing http headers */
    ch_str_t header_field;
    ch_str_t header_value;
} ch_http_client_t;


typedef void(*ch_http_handler_t)(ch_http_client_t *client);


int ch_http_client_init(ch_http_client_t *client, ch_http_server_t *server);
void ch_http_client_write(ch_http_client_t *client, ch_str_t *str);
void ch_http_client_finish(ch_http_client_t *client, ch_str_t *str);
void ch_http_client_free(ch_http_client_t *client);



#endif /* end of include guard */
