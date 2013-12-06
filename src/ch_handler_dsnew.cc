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
#include <ch_log.h>
#include <ch_protocol.h>
#include <ch_client.h>
#include <ch_dataset.h>
#include <ch_chimpd.h>

extern ch_chimpd_t chimpd;

void ch_handler_dsnew(chimp::net::Client *client, ch_message_t *message)
{
    std::map<std::string, ch_dataset_t*>::iterator iter;
    ch_message_dsnew_t *msg = dynamic_cast<ch_message_dsnew_t*>(message);

    iter = chimpd.datasets.find(msg->name);
    if (iter != chimpd.datasets.end()) {
        client->Write(CH_RESPONSE_CODE_USER_ERROR, "dataset exists");
        return;
    }

    ch_dataset_t *dataset = new ch_dataset_t;

    chimpd.datasets[msg->name] = dataset;
    CH_LOG_DEBUG("created new dataset: %s, cols: %d", msg->name.c_str(), msg->num_columns);
    client->Write(CH_RESPONSE_CODE_OK, NULL);
}
