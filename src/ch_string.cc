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
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#ifdef HAVE_BSD_STRING_H
#include <bsd/string.h>
#endif
#include <ch_string.h>


void ch_str_init_empty(ch_str_t *str)
{
    assert(str);
    memset(str, 0, sizeof(ch_str_t));
}


void ch_str_init(ch_str_t *str, const char *s)
{
    assert(str);
    assert(s);
    str->data = strdup(s);
    str->len = strlen(s);
    str->size = str->len + 1;
}


void ch_str_linit(ch_str_t *str, const char *s, size_t len)
{
    assert(str);
    assert(s);
    str->size = len + 1;
    str->len = len;
    str->data = (char*)malloc(str->size);
    strlcpy(str->data, s, str->size);
}


void ch_str_init_alloc(ch_str_t *str, size_t size)
{
    assert(str);
    str->len = 0;
    str->size = size;
    str->data = (char*)malloc(size);
}


void ch_str_free(ch_str_t *str)
{
    assert(str);
    free(str->data);
    str->data = NULL;
    str->len = str->size = 0;
}


void ch_str_lcat(ch_str_t *str, const char *s, size_t len)
{
    assert(str);
    assert(s);

    size_t new_size;
    char *new_data;

    if (len + str->len < str->size) {
        /* len + 1 since we have 1 extra byte for \0
         * in the buffer
         * */
        strlcpy(str->data + str->len, s, len + 1);
        str->len += len;
        return;
    }

    new_size = (str->size + len) * 2;
    new_data = (char*)malloc(new_size);
    strlcpy(new_data, str->data, new_size);
    strlcpy(new_data + str->len, s, new_size - str->len);
    free(str->data);
    str->size = new_size;
    str->len += len;
    str->data = new_data;
}


void ch_str_cat(ch_str_t *str, const char *s)
{
    ch_str_lcat(str, s, strlen(s));
}


int ch_str_to_long(const char *s, long *out)
{
    char *endptr = NULL;

    if (!s || !*s) {
        return -1;
    }

    *out = strtol(s, &endptr, 10);
    if (*endptr != '\0') {
        return -1;
    }

    return 0;
}


int ch_str_to_double(const char *s, double *out)
{
    char *endptr = NULL;

    if (!s || !*s) {
        return -1;
    }

    *out = strtod(s, &endptr);
    if (*endptr != '\0') {
        return -1;
    }

    return 0;
}


void ch_str_printf(ch_str_t *str, const char *format, ...)
{
    va_list ap;
    char *ret;
    int copied;

    va_start(ap, format);
    copied = vasprintf(&ret, format, ap);
    va_end(ap);

    if (copied == -1) {
        return;
    }

    ch_str_lcat(str, ret, copied);
    free(ret);
}
