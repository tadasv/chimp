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
#include <string.h>
#include <murmur.h>
#include <ch_hash_table.h>


#define MURMUR_SALT 0xdeadbeef


void ch_hash_table_init(ch_hash_table_t *ht, size_t size)
{
    assert(ht);

    size_t i;
    ht->size = size;
    ht->buckets = malloc(sizeof(ch_hash_element_t*) * size);

    for (i = 0; i < size; i++) {
        ht->buckets[i] = NULL;
    }
}


void ch_hash_table_add(ch_hash_table_t *ht, void *key, size_t key_len, void *data)
{
    uint64_t hash[2] = {0,0};
    uint32_t bucket_index;
    ch_hash_element_t *elem;

    assert(ht);

    MurmurHash3_x64_128(key, key_len, MURMUR_SALT, hash);
    bucket_index = hash[0] % ht->size;

    if (ht->buckets[bucket_index]) {
        // on collisions release old data.
        free(ht->buckets[bucket_index]);
    }

    elem = malloc(sizeof(ch_hash_element_t) + key_len);
    elem->data = data;
    elem->length = key_len;
    memcpy(elem->key, key, key_len);
    ht->buckets[bucket_index] = elem;
}


ch_hash_element_t* ch_hash_table_find(ch_hash_table_t *ht, void *key, size_t key_len)
{
    uint64_t hash[2] = {0,0};
    uint32_t bucket_index;

    assert(ht);
    assert(key);

    MurmurHash3_x64_128(key, key_len, MURMUR_SALT, hash);
    bucket_index = hash[0] % ht->size;
    return ht->buckets[bucket_index];
}


void ch_hash_table_free(ch_hash_table_t *ht)
{
    assert(ht);
    size_t i;

    for (i = 0; i < ht->size; i++) {
        if (ht->buckets[i]) {
            free(ht->buckets[i]);
        }
    }

    free(ht->buckets);
}
