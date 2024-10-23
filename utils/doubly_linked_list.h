#include <stdlib.h>
#include "commons.h"
#include "logger.h"

struct DoublyLinkedList{
    void* data;
    struct DoublyLinkedList* next;
    struct DoublyLinkedList* prev;
} typedef DoublyLinkedList;

DoublyLinkedList* allocate_list_mem();

DoublyLinkedList* get_list_head(DoublyLinkedList* doubly_linked_list);

DoublyLinkedList* get_list_tail(DoublyLinkedList* doubly_linked_list);

int get_list_length(DoublyLinkedList* doubly_linked_list);

int is_list_empty(DoublyLinkedList* doubly_linked_list);

int list_add(DoublyLinkedList* doubly_linked_list, void* data);

int merge_lists(DoublyLinkedList* doubly_linked_list1, DoublyLinkedList doubly_linked_list2);

int is_list_contains_string(DoublyLinkedList* doubly_linked_list, char* string);

int free_list(DoublyLinkedList* doubly_linked_list);

