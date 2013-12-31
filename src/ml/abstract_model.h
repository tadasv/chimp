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
#ifndef CH_INCLUDE_GUARD_98DE2924_91FE_4315_BBFD_5BA0F8C2DCF0
#define CH_INCLUDE_GUARD_98DE2924_91FE_4315_BBFD_5BA0F8C2DCF0

#include <msgpack.h>
#include <string>
#include <memory>

namespace chimp {
namespace ml {
namespace model {

class AbstractModelResult {
    public:
        virtual ~AbstractModelResult();
        virtual msgpack_sbuffer *ToMessagePack() = 0;
};


class AbstractModelInput {
    public:
        virtual ~AbstractModelInput();
        virtual int FromMessagePack(const msgpack_object *msg) = 0;
};


class AbstractModel {
    public:
        virtual ~AbstractModel();
        const std::string &GetName() const { return name_; };
        void SetName(std::string name) { name_ = name; };
        virtual int Build(const AbstractModelInput *input) = 0;
        virtual std::shared_ptr<AbstractModelResult> Predict(const AbstractModelInput *input) const = 0;
    protected:
        std::string name_;
};

}; // namespace model
}; // namespace ml
}; // namespace chimp

#endif /* end of include guard */
