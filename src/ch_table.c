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
#include <ch_table.h>


void ch_table_init(ch_table_t *table, size_t cols, size_t index_size)
{
    assert(table);
    assert(index_size > 0);
    table->cols_per_row = cols;
    table->index_size = index_size;
    table->nrows = 0;
    table->index = malloc(sizeof(ch_row_t*) * index_size);
    memset(table->index, 0, sizeof(ch_row_t*) * index_size);
}


void ch_table_free(ch_table_t *table)
{
    assert(table);
    size_t i;

    for (i = 0; i < table->index_size; i++) {
        if (table->index[i]) {
            ch_row_free(table->index[i]);
        }
    }
}


int ch_table_set(ch_table_t *table, off_t row, off_t col, double value)
{
    assert(table);

    ch_row_t *new_row;

    if (col >= table->cols_per_row) {
        return -1;
    }

    if (row < table->nrows) {
        return ch_row_set(table->index[row], col, value);
    }

    if (row > table->nrows) {
        /* we do not allow to have gaps between rows */
        return -1;
    }

    /* make sure that we have enough room in index for a new row */
    if (row >= table->index_size) {
        table->index = realloc(table->index, sizeof(ch_row_t*) * 2 * table->index_size);
        table->index_size = table->index_size * 2;
    }

    new_row = malloc(sizeof(ch_row_t));
    ch_row_init(new_row, table->cols_per_row);

    assert(row == table->nrows);
    table->index[row] = new_row;
    table->nrows++;

    return ch_row_set(new_row, col, value);
}


int ch_table_get(ch_table_t *table, off_t row, off_t col, double *out)
{
    assert(table);
    assert(out);

    ch_row_t *row_ptr;

    if (col >= table->cols_per_row) {
        return -1;
    }

    if (row >= table->nrows) {
        return -1;
    }

    row_ptr = table->index[row];
    return ch_row_get(row_ptr, col, out);
}
