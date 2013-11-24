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
#include <string.h>
#include <ch_row.h>


void ch_row_init(ch_row_t *row, size_t ncols)
{
    assert(row);
    row->ncols = ncols;
    row->data = (double*)malloc(sizeof(double) * ncols);
    memset(row->data, 0, sizeof(double) * ncols);
}


void ch_row_free(ch_row_t *row)
{
    assert(row);
    free(row->data);
}


int ch_row_get(ch_row_t *row, off_t slot, double *out)
{
    assert(row);
    assert(out);

    if (slot >= row->ncols) {
        return -1;
    }

    *out = row->data[slot];

    return 0;
}


int ch_row_set(ch_row_t *row, off_t slot, double value)
{
    assert(row);

    if (slot >= row->ncols) {
        return -1;
    }

    row->data[slot] = value;

    return 0;
}
