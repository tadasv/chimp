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
#include <assert.h>
#include <stdlib.h>
#include <ch_list.h>


void ch_list_init_empty(ch_list_t *list)
{
    assert(list);
    list->head = NULL;
    list->tail = NULL;
}


void ch_list_init(ch_list_t *list, void *data)
{
    assert(list);
    ch_node_t *node = malloc(sizeof(ch_node_t));
    node->prev = NULL;
    node->next = NULL;
    node->data = data;

    list->head = node;
    list->tail = node;
}


void ch_list_free(ch_list_t *list)
{
    assert(list);
    ch_node_t *node;

    while (list->head) {
        node = list->head->next;
        free(list->head);
        list->head = node;
    }

    list->head = NULL;
    list->tail = NULL;
}


void ch_list_append(ch_list_t *list, void *data)
{
    assert(list);
    ch_node_t *node = malloc(sizeof(ch_node_t));
    node->prev = list->tail;
    node->next = NULL;
    node->data = data;
    if (list->tail) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
}
