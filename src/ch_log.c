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
#include <stdarg.h>
#include <stdio.h>
#include <ch_log.h>


static const char *error_levels[] = {
    NULL,
    "error",
    "info",
    "debug"
};


void ch_log_write(unsigned int level, const char *fmt, ...)
{
    int written;
    char log_buffer[8192];
    char *p;
    va_list vargs;

    written = snprintf(log_buffer, sizeof(log_buffer), "chimp: [%s] ", error_levels[level]);
    p = log_buffer + written;
    va_start(vargs, fmt);
    written = vsnprintf(p, sizeof(log_buffer) - written, fmt, vargs);
    va_end(vargs);
    p += written;
    *p++ = '\n';
    *p = '\0';

    fprintf(stdout, "%s", log_buffer);
}
