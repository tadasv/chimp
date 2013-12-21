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
namespace dataset {

Dataset::Dataset(const std::string &name, uint32_t ncols)
{
    name_ = name;
    max_ncols_ = ncols;
    data_.set_size(0, ncols);
}


std::string Dataset::GetName()
{
    return name_;
}


Dimensions Dataset::GetDimensions() const
{
    Dimensions dims;
    dims.cols = data_.n_cols;
    dims.rows = data_.n_rows;
    return dims;
}


int Dataset::SetItem(uint32_t row, uint32_t col, double value)
{
    if (row >= data_.n_rows || col >= data_.n_cols) {
        return -1;
    }

    data_(row, col) = value;

    return 0;
}


int Dataset::GetItem(uint32_t row, uint32_t col, double *out)
{
    if (row >= data_.n_rows || col >= data_.n_cols) {
        return -1;
    }

    *out = data_(row, col);
    return 0;
}


int Dataset::Resize(uint32_t rows, uint32_t cols)
{
    data_.resize(rows, cols);
    max_ncols_ = cols;
    return 0;
}


int Dataset::Append(const std::vector<double> &new_data)
{
    // vector length must be a multiple of number of columns
    if (new_data.size() % data_.n_cols != 0) {
        return -1;
    }

    uint32_t new_rows = new_data.size() / data_.n_cols;
    if (new_rows == 0) {
        return 0;
    }

    // expand dataset to fit new data
    uint32_t starting_row = data_.n_rows;
    uint32_t starting_column = 0;
    Resize(data_.n_rows + new_rows, data_.n_cols);

    for (uint32_t i = 0; i < new_data.size(); ++i) {
        data_(starting_row, starting_column++) = new_data[i];
        if (starting_column == data_.n_cols) {
            starting_column = 0;
            starting_row++;
        }
    }

    return 0;
}


}; // namespace dataset
}; // namespace db
}; // namespace chimp
