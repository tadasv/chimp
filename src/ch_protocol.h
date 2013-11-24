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
#ifndef CH_INCLUDE_GUARD_B4343471_000F_4433_B0C5_F24EDD78B69E
#define CH_INCLUDE_GUARD_B4343471_000F_4433_B0C5_F24EDD78B69E

#include <string>
#include <msgpack.h>

typedef enum ch_response_code_t {
    CH_RESPONSE_CODE_OK = 200,
    CH_RESPONSE_CODE_USER_ERROR = 400,
    CH_RESPONSE_CODE_SERVER_ERROR = 500
} ch_response_code_t;


typedef enum ch_command_t {
    CH_COMMAND_PING,
    CH_COMMAND_DSNEW,
} ch_command_t;


struct ch_message_t {
    virtual msgpack_sbuffer *serialize() = 0;
    virtual int unserialize(const msgpack_unpacked *msg) = 0;
};


struct ch_response_message_t : ch_message_t {
    ch_response_code_t code;
    std::string error;

    ch_response_message_t();
    ch_response_message_t(ch_response_code_t code);

    msgpack_sbuffer *serialize();
    int unserialize(const msgpack_unpacked *msg);
};


/*
 * DSNEW command
 *
 * ["DSNEW", ["dataset name", 5]]
 */
struct ch_message_dsnew_t : ch_message_t {
    std::string name;
    unsigned int num_columns;

    msgpack_sbuffer *serialize();
    int unserialize(const msgpack_unpacked *msg);
};

#endif /* end of include guard */
