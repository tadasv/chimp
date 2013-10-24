/**
 * Copyright (C) Tadas Vilkeliskis
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


void ch_hash_table_init(ch_hash_table_t *ht, size_t size);
void ch_hash_table_add(ch_hash_table_t *ht, void *key, size_t key_len, void *data);
ch_hash_element_t* ch_hash_table_find(ch_hash_table_t *ht, void *key, size_t key_len);
void ch_hash_table_free(ch_hash_table_t *ht);


#endif /* end of include guard */
