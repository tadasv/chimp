/**
 * Copyright (C) Tadas Vilkeliskis
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

    ht->size = size;
    ht->buckets = malloc(sizeof(ch_hash_element_t*) * size);

    for (size_t i = 0; i < size; i++) {
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

    for (size_t i = 0; i < ht->size; i++) {
        if (ht->buckets[i]) {
            free(ht->buckets[i]);
        }
    }

    free(ht->buckets);
}
