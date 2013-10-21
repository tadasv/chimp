/**
 * Copyright (C) Tadas Vilkeliskis
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
    list->tail->next = node;
    list->tail = node;
}
