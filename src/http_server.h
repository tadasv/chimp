/**
 * Copyright (C) Tadas Vilkeliskis
 */
#ifndef CH_INCLUDE_GUARD_CEC85A6A_601A_4964_9263_8B2D87B43FDA
#define CH_INCLUDE_GUARD_CEC85A6A_601A_4964_9263_8B2D87B43FDA


#include <uv.h>
#include "http_parser.h"


typedef struct ch_http_server_settings_ {
    int port;
    int socket_backlog;
} ch_http_server_settings_t;


typedef struct ch_http_server_ {
    uv_loop_t *loop;
    uv_tcp_t handle;
    http_parser_settings parser_settings;
    ch_http_server_settings_t *settings;
} ch_http_server_t;


int ch_http_server_init(ch_http_server_t *server, ch_http_server_settings_t *settings, uv_loop_t *loop);
int ch_http_server_start(ch_http_server_t *server);


#endif /* end of include guard */
