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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ch_chimpd.h>
#include <ch_dataset.h>
#include <ch_http_client.h>

extern ch_chimpd_t chimpd;


static void create_dataset(ch_http_client_t *client)
{
    const char *dataset_name = ch_http_message_get_header(&client->request, "Dataset-Name");
    ch_dataset_t *new_dataset;

    if (!dataset_name) {
        ch_http_client_write_status_line(client, 400);
        ch_http_client_finish(client, NULL);
        return;
    }

    if (ch_hash_table_multi_find(&chimpd.datasets,
                                 (void*)dataset_name,
                                 strlen(dataset_name))) {
        ch_http_client_write_status_line(client, 400);
        ch_http_client_finish(client, NULL);
        return;
    }

    /* we don't need to copy anything, just take ownership
     * of the memory */
    new_dataset = (ch_dataset_t*)malloc(sizeof(ch_dataset_t));
    ch_dataset_init(new_dataset);
    new_dataset->data = client->request.csv_table;
    client->request.csv_table = NULL;

    ch_hash_table_multi_add(&chimpd.datasets,
                            (void*)dataset_name,
                            strlen(dataset_name),
                            new_dataset);

    ch_http_client_write_status_line(client, 200);
    ch_http_client_finish(client, NULL);
}


static void delete_dataset(ch_http_client_t *client)
{
    const char *dataset_name = ch_http_message_get_header(&client->request, "Dataset-Name");
    ch_dataset_t *dataset;

    if (!dataset_name) {
        ch_http_client_write_status_line(client, 400);
        ch_http_client_finish(client, NULL);
        return;
    }

    dataset = (ch_dataset_t*)ch_hash_table_multi_find(&chimpd.datasets,
                                       (void*)dataset_name,
                                       strlen(dataset_name));
    if (dataset) {
        ch_hash_table_multi_delete(&chimpd.datasets,
                                   (void*)dataset_name,
                                   strlen(dataset_name));
        ch_dataset_free(dataset);
    }

    ch_http_client_write_status_line(client, 200);
    ch_http_client_finish(client, NULL);
}


static void list_datasets(ch_http_client_t *client)
{
    ch_str_t response;
    size_t i;
    char content_length[16];

    ch_hash_element_multi_t *elem;
    ch_str_init_alloc(&response, 2048);

    ch_http_client_write_status_line(client, 200);

    for (i = 0; i < chimpd.datasets.size; i++) {
        elem = chimpd.datasets.buckets[i];
        while (elem) {
            ch_str_lcat(&response, (const char*)elem->key, elem->length);
            ch_str_cat(&response, "\n");
            elem = elem->next;
        }
    }

    if (response.len) {
        snprintf(content_length, sizeof(content_length), "%zd", response.len); 
        ch_http_client_write_header(client, "Content-Length", content_length, 1);
        ch_http_client_finish(client, &response);
    } else {
        ch_http_client_finish(client, NULL);
    }
}


void ch_http_handler_dataset(ch_http_client_t *client)
{
    switch (client->request.method) {
        case CH_HTTP_REQUEST_METHOD_POST:
            create_dataset(client);
            break;
        case CH_HTTP_REQUEST_METHOD_GET:
            list_datasets(client);
            break;
        case CH_HTTP_REQUEST_METHOD_DELETE:
            delete_dataset(client);
            break;
        default:
            ch_http_client_write_status_line(client, 500);
            ch_http_client_finish(client, NULL);
            break;
    }
}
