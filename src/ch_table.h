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
#ifndef INCLUDE_GUARD_9A37DF4D_3D83_46F5_AD4C_E630D5F98794
#define INCLUDE_GUARD_9A37DF4D_3D83_46F5_AD4C_E630D5F98794

#include <ch_row.h>

typedef struct ch_table_ {
    size_t cols_per_row;
    size_t index_size;
    size_t nrows;
    ch_row_t **index;
} ch_table_t;


void ch_table_init(ch_table_t *table, size_t cols, size_t index_size);
void ch_table_free(ch_table_t *table);

int ch_table_set(ch_table_t *table, off_t row, off_t col, double value);
int ch_table_get(ch_table_t *table, off_t row, off_t col, double *out);

#endif /* end of include guard */
