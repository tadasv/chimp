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
#ifndef INCLUDE_GUARD_47373DC1_5404_46C7_BC88_CF4E336A0672
#define INCLUDE_GUARD_47373DC1_5404_46C7_BC88_CF4E336A0672


typedef struct ch_hash_element_ {
    void *data;
    size_t length;
    // key is not null terminated
    unsigned char key[];
} ch_hash_element_t;


// simple hash table which does not handle collisions
typedef struct ch_hash_table_ {
    ch_hash_element_t **buckets;
    size_t size;
} ch_hash_table_t;


typedef struct ch_hash_element_multi_ {
    void *data;
    size_t length;
    struct ch_hash_element_multi_ *next;
    struct ch_hash_element_multi_ *prev;
    unsigned char key[];
} ch_hash_element_multi_t;


typedef struct ch_hash_table_multi_ {
    ch_hash_element_multi_t **buckets;
    size_t size;
} ch_hash_table_multi_t;


void ch_hash_table_init(ch_hash_table_t *ht, size_t size);
void ch_hash_table_multi_init(ch_hash_table_multi_t *ht, size_t size);

void ch_hash_table_add(ch_hash_table_t *ht, void *key, size_t key_len, void *data);
void ch_hash_table_multi_add(ch_hash_table_multi_t *ht, void *key, size_t key_len, void *data);

void *ch_hash_table_find(ch_hash_table_t *ht, void *key, size_t key_len);
void *ch_hash_table_multi_find(ch_hash_table_multi_t *ht, void *key, size_t key_len);

void ch_hash_table_free(ch_hash_table_t *ht);
void ch_hash_table_multi_free(ch_hash_table_multi_t *ht);


#endif /* end of include guard */
