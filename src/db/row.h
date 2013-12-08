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
#ifndef CH_INCLUDE_GUARD_2D2C6CF5_924C_497A_BA31_9CFF43C35E97
#define CH_INCLUDE_GUARD_2D2C6CF5_924C_497A_BA31_9CFF43C35E97

#include <cstdint>
#include <memory>


namespace chimp {
namespace db {

class DatasetRow {
    public:
        DatasetRow(uint32_t ncols);
        ~DatasetRow();

        int SetColumn(uint32_t slot, double value);
        int GetColumn(uint32_t slot, double *out);
        uint32_t NumColumns();
    private:
        uint32_t ncols_;
        std::unique_ptr<double, std::default_delete<double[]>> data_;
};

}
}



#endif /* end of include guard */
