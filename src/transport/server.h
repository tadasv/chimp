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
#ifndef CH_INCLUDE_GUARD_E328F9CB_3BA1_42DB_978D_FE48847105B7
#define CH_INCLUDE_GUARD_E328F9CB_3BA1_42DB_978D_FE48847105B7

#include <uv.h>
#include <map>


namespace chimp {
namespace transport {

class Server {
    public:
        class ServerSettings {
            public:
                ServerSettings() : port(8000), socket_backlog(128) {};

                int port;
                int socket_backlog;
        };

        enum Command {
            PING,
            DSNEW
        };

        Server(ServerSettings settings, uv_loop_t *loop);
        int Start();
    public:
        uv_loop_t *loop;
        uv_tcp_t handle;
        ServerSettings settings_;
        std::map<std::string, Command> commands;
};

}
}


#endif /* end of include guard */
