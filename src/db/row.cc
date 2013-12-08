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
#include <cstring>
#include "db/row.h"


namespace chimp {
namespace db {

DatasetRow::DatasetRow(uint32_t ncols) : ncols_(ncols), data_(new double[ncols])
{
    memset(data_.get(), 0, sizeof(double) * ncols_);
}


DatasetRow::~DatasetRow()
{
}


uint32_t DatasetRow::NumColumns()
{
    return ncols_;
}


int DatasetRow::SetColumn(uint32_t slot, double value)
{
    if (slot >= ncols_) {
        return -1;
    }

    double *data = data_.get();
    data[slot] = value;

    return 0;
}


int DatasetRow::GetColumn(uint32_t slot, double *out)
{
    if (out == NULL) {
        return -1;
    }

    if (slot >= ncols_) {
        return -1;
    }

    double *data = data_.get();
    *out = data[slot];

    return 0;
}


}
}
