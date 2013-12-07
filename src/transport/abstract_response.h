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
#ifndef CH_INCLUDE_GUARD_C18B88FC_000F_4EAF_9E1A_60C18539795E
#define CH_INCLUDE_GUARD_C18B88FC_000F_4EAF_9E1A_60C18539795E

#include <msgpack.h>

namespace chimp {
namespace transport {

enum ResponseCode {
    RESPONSE_CODE_OK = 200,
    RESPONSE_CODE_USER_ERROR = 400,
    RESPONSE_CODE_SERVER_ERROR  = 500
};

class AbstractResponse {
    public:
        virtual ~AbstractResponse();
        virtual msgpack_sbuffer *ToMessagePack() = 0;
    protected:
        AbstractResponse();
};

}
}



#endif /* end of include guard */
