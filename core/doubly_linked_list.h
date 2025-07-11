#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
#include <stdlib.h>
#include "../utils/boolean.h"
#include "../utils/logger.h"


typedef struct DoublyLinkedList{
    void* data;                       /* Pointer to the data stored in the node */
    struct DoublyLinkedList* next;    /* Pointer to the next node in the list */
    struct DoublyLinkedList* prev;    /* Pointer to the previous node in the list */
} DoublyLinkedList;


/*Allocates memory for a new  doubly linked list */
DoublyLinkedList* allocate_node_mem();

/*Returns the head  of the doubly linked list */
DoublyLinkedList* get_list_head(DoublyLinkedList* doubly_linked_list);

/* Returns the tail of the doubly linked list */
DoublyLinkedList* get_list_tail(DoublyLinkedList* doubly_linked_list);

/* Returns the length (number of nodes) in the doubly linked list */
int get_list_length(DoublyLinkedList* doubly_linked_list);
int get_list_length_from_current(DoublyLinkedList* doubly_linked_list);


/* Adds a new node to the tail of the doubly linked list */
int add_to_list(DoublyLinkedList* doubly_linked_list, void* data);

/*deletes list content w/o free its memory for re-use*/
int clear_list(DoublyLinkedList* doubly_linked_list, void (*free_node_data)(void*));

/* returns 1 if the list is empty, otherwise returns 0 */
int is_list_empty(DoublyLinkedList* doubly_linked_list);

/* checks if the list contains the given string; returns 1 if found, 0 otherwise */
int is_list_contains_string(DoublyLinkedList* doubly_linked_list, char* string);

/* Frees all nods in the list and their data, using a callback function to free the data */
int free_list(DoublyLinkedList** doubly_linked_list, void (*free_node_data)(void*));

#endif