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
#ifndef CH_INCLUDE_GUARD_8FE7CAB5_FA4B_4AD5_B11B_9FEDF6C9395F
#define CH_INCLUDE_GUARD_8FE7CAB5_FA4B_4AD5_B11B_9FEDF6C9395F

#include <string>
#include <memory>
#include "transport/client.h"
#include "transport/command/abstract_command.h"
#include "ml/abstract_model.h"

namespace chimp {
namespace transport {
namespace command {

class ModelPredict : public AbstractCommand {
    public:
        class Response : public AbstractResponse {
            public:
                Response();
                void SetResponse(const msgpack_sbuffer *data);
                msgpack_sbuffer *ToMessagePack();
            private:
                std::unique_ptr<char[]> serialized_data_;
                uint32_t data_size_;
        };

        ModelPredict(chimp::transport::Client *client);
        int Execute();
        int FromMessagePack(const msgpack_unpacked *msg);
        msgpack_sbuffer *ToMessagePack();
    private:
        chimp::transport::Client *client_;
        std::string model_name_;
        std::shared_ptr<chimp::ml::model::AbstractModelInput> model_input_;
};

}; // namespace command
}; // namespace transport
}; // namespace chimp

#endif /* end of include guard */
