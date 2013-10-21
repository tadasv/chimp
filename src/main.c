/**
 * Copyright (C) Tadas Vilkeliskis
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>
#include <ch_log.h>
#include <ch_http_server.h>


typedef struct {
    int port;
} chimp_settings_t;

static chimp_settings_t chimp_settings;


static void print_version()
{
    CH_LOG_INFO("chimp %s", VERSION);
    exit(0);
}


static void usage(const char *program_name)
{
    printf("usage: %s [option] [arg] ...\n", program_name);
    printf("Options and arguments:\n"
           "-p      : server port to bind to\n"
           "-h      : print this help screen and exit\n"
           "-v      : print version and exit\n"
           );

    exit(0);
}


static void parse_command_line(int argc, char * const argv[])
{
    int cmd_option;
    extern char *optarg;

    memset(&chimp_settings, 0, sizeof(chimp_settings));
    while ((cmd_option = getopt(argc, argv, "hvp:")) != -1) {
        switch (cmd_option) {
            case 'v':
                print_version();
                break;
            case 'p':
                chimp_settings.port = strtol(optarg, NULL, 10);
                break;
            case 'h':
            default:
                usage(argv[0]);
        }
    }

    if (chimp_settings.port <= 0) {
        printf("missing port number\n");
        usage(argv[0]);
    }
}


int main(int argc, char * const argv[])
{
    uv_loop_t *loop = uv_default_loop();
    ch_http_server_t http_server;
    ch_http_server_settings_t http_server_settings;

    parse_command_line(argc, argv);

    http_server_settings.port = chimp_settings.port;
    http_server_settings.socket_backlog = 128;

    ch_http_server_init(&http_server, &http_server_settings, loop);
    if (ch_http_server_start(&http_server)) {
        return -1;
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
