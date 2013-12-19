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
#ifndef CH_INCLUDE_GUARD_1A2B85D6_395E_4607_9A01_56B5F4DF6CF4
#define CH_INCLUDE_GUARD_1A2B85D6_395E_4607_9A01_56B5F4DF6CF4

#include <cstdint>
#include <string>


namespace chimp {
namespace db {
namespace dataset {

struct Dimensions {
    uint64_t cols;
    uint64_t rows;
};


class AbstractDataset {
    public:
        virtual ~AbstractDataset();

        virtual std::string GetName() = 0;
        virtual Dimensions GetDimensions() const = 0;
};

};
}; // namespace db
}; // namespace chimp


#endif /* end of include guard */
