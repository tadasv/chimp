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
#ifndef CH_INCLUDE_GUARD_1A0C8CC8_A220_4850_95B7_56AAF11BA7E4
#define CH_INCLUDE_GUARD_1A0C8CC8_A220_4850_95B7_56AAF11BA7E4

#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "db/row.h"


namespace chimp {
namespace db {

class Dataset {
    public:
        Dataset(const std::string &name, uint32_t ncols);

        int SetItem(uint32_t row, uint32_t col, double value);
        int GetItem(uint32_t row, uint32_t col, double *out);
        uint64_t NumRows();
        std::string GetName();
    private:
        std::string name_;
        uint64_t ncols_per_row_;
        std::vector<std::unique_ptr<DatasetRow>> rows_;

};

}
}


#endif /* end of include guard */
