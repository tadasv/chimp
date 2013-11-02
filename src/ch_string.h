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
#ifndef INCLUDE_GUARD_8BC91D14_BE41_4A7B_9368_D6130A51CB07
#define INCLUDE_GUARD_8BC91D14_BE41_4A7B_9368_D6130A51CB07


typedef struct ch_str_ {
    size_t len;
    size_t size;
    char *data;
} ch_str_t;


typedef struct ch_keyval_ {
    ch_str_t key;
    ch_str_t value;
} ch_keyval_t;


void ch_str_init_empty(ch_str_t *str);
void ch_str_init(ch_str_t *str, const char *s);
void ch_str_linit(ch_str_t *str, const char *s, size_t len);
void ch_str_init_alloc(ch_str_t *str, size_t size);
void ch_str_cat(ch_str_t *str, const char *s);
void ch_str_lcat(ch_str_t *str, const char *s, size_t len);
void ch_str_free(ch_str_t *str);

int ch_str_to_long(const char *s, long *out);
int ch_str_to_double(const char *s, double *out);


#endif /* end of include guard */
