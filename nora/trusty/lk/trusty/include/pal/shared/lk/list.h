// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// LK's intrusive doubly linked list, as the Trusty user library provides it.
// This is a plain data structure with no TEE behind it, so it is complete.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <zircon/compiler.h>

__BEGIN_CDECLS

#ifndef containerof
#define containerof(ptr, type, member) \
    ((type*)((uintptr_t)(ptr) - offsetof(type, member)))
#endif

struct list_node {
    struct list_node* prev;
    struct list_node* next;
};

typedef struct list_node list_node_t;

#define LIST_INITIAL_VALUE(list) { &(list), &(list) }
#define LIST_INITIAL_CLEARED_VALUE { NULL, NULL }

static inline void list_initialize(struct list_node* list) {
    list->prev = list->next = list;
}

static inline void list_clear_node(struct list_node* item) {
    item->prev = item->next = NULL;
}

static inline bool list_in_list(const struct list_node* item) {
    return !(item->prev == NULL && item->next == NULL);
}

static inline void list_add_head(struct list_node* list, struct list_node* item) {
    item->next = list->next;
    item->prev = list;
    list->next->prev = item;
    list->next = item;
}

#define list_add_after(entry, new_entry) list_add_head(entry, new_entry)

static inline void list_add_tail(struct list_node* list, struct list_node* item) {
    item->prev = list->prev;
    item->next = list;
    list->prev->next = item;
    list->prev = item;
}

#define list_add_before(entry, new_entry) list_add_tail(entry, new_entry)

static inline void list_delete(struct list_node* item) {
    item->next->prev = item->prev;
    item->prev->next = item->next;
    item->prev = item->next = NULL;
}

static inline struct list_node* list_remove_head(struct list_node* list) {
    if (list->next != list) {
        struct list_node* item = list->next;
        list_delete(item);
        return item;
    }
    return NULL;
}

#define list_remove_head_type(list, type, element) ({ \
    struct list_node* __nod = list_remove_head(list); \
    type* __t; \
    if (__nod) \
        __t = containerof(__nod, type, element); \
    else \
        __t = (type*)0; \
    __t; \
})

static inline struct list_node* list_remove_tail(struct list_node* list) {
    if (list->prev != list) {
        struct list_node* item = list->prev;
        list_delete(item);
        return item;
    }
    return NULL;
}

#define list_remove_tail_type(list, type, element) ({ \
    struct list_node* __nod = list_remove_tail(list); \
    type* __t; \
    if (__nod) \
        __t = containerof(__nod, type, element); \
    else \
        __t = (type*)0; \
    __t; \
})

static inline struct list_node* list_peek_head(struct list_node* list) {
    return list->next != list ? list->next : NULL;
}

#define list_peek_head_type(list, type, element) ({ \
    struct list_node* __nod = list_peek_head(list); \
    type* __t; \
    if (__nod) \
        __t = containerof(__nod, type, element); \
    else \
        __t = (type*)0; \
    __t; \
})

static inline struct list_node* list_peek_tail(struct list_node* list) {
    return list->prev != list ? list->prev : NULL;
}

#define list_peek_tail_type(list, type, element) ({ \
    struct list_node* __nod = list_peek_tail(list); \
    type* __t; \
    if (__nod) \
        __t = containerof(__nod, type, element); \
    else \
        __t = (type*)0; \
    __t; \
})

static inline struct list_node* list_next(struct list_node* list,
                                          struct list_node* item) {
    if (item->next != list)
        return item->next;
    return NULL;
}

#define list_next_type(list, item, type, element) ({ \
    struct list_node* __nod = list_next(list, item); \
    type* __t; \
    if (__nod) \
        __t = containerof(__nod, type, element); \
    else \
        __t = (type*)0; \
    __t; \
})

// Iterates over the list nodes; the node must not be removed in the body.
#define list_for_every(list, node) \
    for (node = (list)->next; node != (list); node = node->next)

// Iterates over the list nodes; the current node may be removed.
#define list_for_every_safe(list, node, temp_node) \
    for (node = (list)->next, temp_node = (node)->next; \
         node != (list); \
         node = temp_node, temp_node = (node)->next)

// Iterates over the containing entries; the entry must not be removed.
#define list_for_every_entry(list, entry, type, member) \
    for ((entry) = containerof((list)->next, type, member); \
         &(entry)->member != (list); \
         (entry) = containerof((entry)->member.next, type, member))

// Iterates over the containing entries; the current entry may be removed.
#define list_for_every_entry_safe(list, entry, temp_entry, type, member) \
    for (entry = containerof((list)->next, type, member), \
         temp_entry = containerof((entry)->member.next, type, member); \
         &(entry)->member != (list); \
         entry = temp_entry, \
         temp_entry = containerof((temp_entry)->member.next, type, member))

static inline bool list_is_empty(const struct list_node* list) {
    return list->next == list;
}

static inline size_t list_length(const struct list_node* list) {
    size_t cnt = 0;
    const struct list_node* node = list;
    for (node = node->next; node != list; node = node->next)
        cnt++;
    return cnt;
}

__END_CDECLS
