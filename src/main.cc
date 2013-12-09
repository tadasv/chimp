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
#include <string.h>
#include <assert.h>
#include <uv.h>
#include "core/logging.h"
#include "transport/server.h"


static void print_version()
{
    CH_LOG_INFO("chimp " << VERSION);
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


int main(int argc, char * const argv[])
{
    uv_loop_t *loop = uv_default_loop();
    int cmd_option;
    extern char *optarg;

    chimp::transport::Server::ServerSettings settings;

    while ((cmd_option = getopt(argc, argv, "hvp:")) != -1) {
        switch (cmd_option) {
            case 'v':
                print_version();
                break;
            case 'p':
                settings.port = strtol(optarg, NULL, 10);
                break;
            case 'h':
            default:
                usage(argv[0]);
        }
    }

    chimp::transport::Server server(settings, loop);
    if (server.Start() != 0) {
        return -1;
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
