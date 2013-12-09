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
#include <iostream>

#include "core/logging.h"

namespace chimp {
namespace core {
namespace logging {


Logger::Logger(Level level)
{
    level_ = level;
}


void Logger::operator()(std::string const &message, char const *function, char const *file, int line)
{
    const char *level = "default";
    switch (level_) {
        case LEVEL_INFO:
            level = "info";
            break;
        case LEVEL_ERROR:
            level = "error";
            break;
        case LEVEL_DEBUG:
            level = "debug";
            break;
    }

    std::cout << "chimp: [" << level << "] [" << function << " " << file << ":" << line << "]: " << message << std::endl;
}


Logger &Info()
{
    static Logger logger(Level::LEVEL_INFO);
    return logger;
}


Logger &Error()
{
    static Logger logger(Level::LEVEL_ERROR);
    return logger;
}


Logger &Debug()
{
    static Logger logger(Level::LEVEL_DEBUG);
    return logger;
}

}
}
}
