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
#include <string>
#include <sstream>

namespace chimp {
namespace core {
namespace logging {

enum Level {
    LEVEL_INFO,
    LEVEL_ERROR,
    LEVEL_DEBUG
};

class Logger {
    public:
        Logger(Level level);

        void operator()(std::string const &message,
                        char const *function,
                        char const *file,
                        int line);
    private:
        Level level_;
};


Logger &Info();
Logger &Error();
Logger &Debug();

#define CH_LOG(Logger_, Message_) \
    Logger_( \
        static_cast<std::ostringstream&>( \
            std::ostringstream().flush() << Message_ \
        ).str(), \
        __FUNCTION__, \
        __FILE__, \
        __LINE__\
        )


#define CH_LOG_INFO(Message_) CH_LOG(chimp::core::logging::Info(), Message_)
#define CH_LOG_ERROR(Message_) CH_LOG(chimp::core::logging::Error(), Message_)

#ifdef NDEBUG
#define CH_LOG_DEBUG(_) do {} while(0)
#else
#define CH_LOG_DEBUG(Message_) CH_LOG(chimp::core::logging::Debug(), Message_)
#endif




}
}
}
