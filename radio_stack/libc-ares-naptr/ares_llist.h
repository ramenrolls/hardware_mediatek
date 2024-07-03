#ifndef __ARES_LLIST_H
#define __ARES_LLIST_H

/* Node definition for circular, doubly-linked list */
struct list_node {
    struct list_node* prev;
    struct list_node* next;
    void* data;
};

void ares__init_list_head(struct list_node* head);

void ares__init_list_node(struct list_node* node, void* d);

int ares__is_list_empty(struct list_node* head);

void ares__insert_in_list(struct list_node* new_node, struct list_node* old_node);

void ares__remove_from_list(struct list_node* node);

void ares__swap_lists(struct list_node* head_a, struct list_node* head_b);

#endif /* __ARES_LLIST_H */
