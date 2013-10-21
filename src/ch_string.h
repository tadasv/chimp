#ifndef INCLUDE_GUARD_8BC91D14_BE41_4A7B_9368_D6130A51CB07
#define INCLUDE_GUARD_8BC91D14_BE41_4A7B_9368_D6130A51CB07


typedef struct ch_str_ {
    size_t len;
    char *data;
} ch_str_t;


typedef struct ch_keyval_ {
    ch_str_t key;
    ch_str_t value;
} ch_keyval_t;


#endif /* end of include guard */
