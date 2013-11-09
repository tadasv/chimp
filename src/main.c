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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>
#include <ch_log.h>
#include <ch_chimpd.h>
#include <ch_http_server.h>


ch_chimpd_t chimpd;


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

    while ((cmd_option = getopt(argc, argv, "hvp:")) != -1) {
        switch (cmd_option) {
            case 'v':
                print_version();
                break;
            case 'p':
                chimpd.settings.port = strtol(optarg, NULL, 10);
                break;
            case 'h':
            default:
                usage(argv[0]);
        }
    }

    if (chimpd.settings.port <= 0) {
        printf("missing port number\n");
        usage(argv[0]);
    }
}


int main(int argc, char * const argv[])
{
    uv_loop_t *loop = uv_default_loop();
    ch_http_server_t http_server;
    ch_http_server_settings_t http_server_settings;

    memset(&chimpd, 0, sizeof(chimpd));
    ch_hash_table_multi_init(&chimpd.datasets, 1024);

    parse_command_line(argc, argv);

    http_server_settings.port = chimpd.settings.port;
    http_server_settings.socket_backlog = 128;

    ch_http_server_init(&http_server, &http_server_settings, loop);
    if (ch_http_server_start(&http_server)) {
        return -1;
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
