/**
 * Copyright (C) Tadas Vilkeliskis
 */
#ifndef INCLUDE_GUARD_9FE9D177_906E_49B7_ABCB_BD41D1B18A9C
#define INCLUDE_GUARD_9FE9D177_906E_49B7_ABCB_BD41D1B18A9C


typedef struct ch_node_ {
    struct ch_node_ *prev;
    struct ch_node_ *next;
    void *data;
} ch_node_t;


typedef struct ch_list_ {
    ch_node_t *head;
    ch_node_t *tail;
} ch_list_t;


void ch_list_init_empty(ch_list_t *list);
void ch_list_init(ch_list_t *list, void *data);
void ch_list_append(ch_list_t *list, void *data);
void ch_list_free(ch_list_t *list);


#endif /* end of include guard */
