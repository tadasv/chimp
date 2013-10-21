#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ch_string.h>


void ch_str_init(ch_str_t *str, const char *s)
{
    assert(str);
    assert(s);
    str->data = strdup(s);
    str->len = strlen(s);
    str->size = str->len + 1;
}


void ch_str_init_alloc(ch_str_t *str, size_t size)
{
    assert(str);
    str->len = 0;
    str->size = size;
    str->data = malloc(size);
}


void ch_str_free(ch_str_t *str)
{
    assert(str);
    free(str->data);
    str->data = NULL;
    str->len = str->size = 0;
}


void ch_str_cat(ch_str_t *str, const char *s)
{
    assert(str);
    assert(s);

    size_t len = strlen(s);
    size_t new_size;
    char *new_data;

    if (len + str->len < str->size) {
        strlcpy(str->data + str->len, s, str->size - str->len);
        str->len += len;
        return;
    }

    new_size = str->size * 2;
    new_data = malloc(new_size);
    strlcpy(new_data, str->data, new_size);
    strlcpy(new_data + str->len, s, new_size - str->len);
    free(str->data);
    str->size = new_size;
    str->len += len;
    str->data = new_data;
}
