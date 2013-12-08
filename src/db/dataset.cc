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
#include "db/dataset.h"


namespace chimp {
namespace db {


Dataset::Dataset(uint32_t ncols)
{
    ncols_per_row_ = ncols;
}


int Dataset::SetItem(uint32_t row, uint32_t col, double value)
{
    if (col >= ncols_per_row_) {
        return -1;
    }

    DatasetRow *row_ptr = NULL;
    if (row < rows_.size()) {
        row_ptr = rows_[row].get();
    } else {
        if (row != rows_.size()) {
            return -1;
        }

        row_ptr = new DatasetRow(ncols_per_row_);
        rows_.push_back(std::unique_ptr<DatasetRow>(row_ptr));
    }

    return row_ptr->SetColumn(col, value);
}


int Dataset::GetItem(uint32_t row, uint32_t col, double *out)
{
    if (col >= ncols_per_row_ || row >= rows_.size()) {
        return -1;
    }

    DatasetRow *row_ptr = rows_[row].get();
    return row_ptr->GetColumn(col, out);
}


uint64_t Dataset::NumRows()
{
    return rows_.size();
}

}
}
