#include <string.h>
#include "doubly_linked_list.h"

DoublyLinkedList* allocate_node_mem(){
    DoublyLinkedList* doublyLinkedList = malloc(sizeof (DoublyLinkedList));

    if(doublyLinkedList == NULL){
        return NULL;
    }

    doublyLinkedList->data = NULL;
    doublyLinkedList->next = NULL;
    doublyLinkedList->prev = NULL;

    return doublyLinkedList;
}

DoublyLinkedList* get_list_head(DoublyLinkedList* doublyLinkedList){
    DoublyLinkedList* current = doublyLinkedList;

    if (current == NULL) {
        return NULL;
    }

    while (current->prev != NULL){
        current = current->prev;
    }
    return current;
}

DoublyLinkedList* get_list_tail(DoublyLinkedList* doublyLinkedList){
    DoublyLinkedList* current = doublyLinkedList;
    if (current == NULL) {
        return NULL;
    }

    while(current->next != NULL){
        current = current->next;
    }
    return current;
}

int get_list_length(DoublyLinkedList* doublyLinkedList){
    int length = 0;
    DoublyLinkedList* current = get_list_head(doublyLinkedList);

    if (doublyLinkedList == NULL) {
        warnf(-1,"List is Null");
        return 0;
    }

    if(current->data != NULL){
        length++;
    }

    while (current->next != NULL){
        length++;
        current = current->next;
    }
    return length;
}

int get_list_length_from_current(DoublyLinkedList* doubly_linked_list){
    int length = 0;
    DoublyLinkedList* current = doubly_linked_list;

    if (doubly_linked_list == NULL) {
        warnf(-1,"List is Null");
        return 0;
    }

    if(current->data != NULL){
        length++;
    }

    while (current->next != NULL){
        length++;
        current = current->next;
    }
    return length;
}

int add_to_list(DoublyLinkedList* doublyLinkedList, void* data){
    DoublyLinkedList* new_node;
    DoublyLinkedList* tail;

    if(is_list_empty(doublyLinkedList)){
        doublyLinkedList->data = data;
        return TRUE;
    }

    tail = get_list_tail(doublyLinkedList);
    new_node = allocate_node_mem();
    new_node->data=data;
    new_node->prev = tail;
    tail->next = new_node;

    return TRUE;
}

int is_list_empty(DoublyLinkedList* doubly_linked_list) {
    if (doubly_linked_list == NULL) {
        return TRUE;
    }
    return get_list_length(doubly_linked_list) == 0;
}

int is_list_contains_string(DoublyLinkedList* doubly_linked_list, char* string){
    while (doubly_linked_list!=NULL){
        if (doubly_linked_list->data != NULL && strcmp((char*)doubly_linked_list->data, string) == 0) {
            return TRUE;
        }
        doubly_linked_list = doubly_linked_list->next;
    }
    return FALSE;
}


int clear_list(DoublyLinkedList* doubly_linked_list, void (*free_node_data)(void*)) {
    DoublyLinkedList* current;
    DoublyLinkedList* next_node;
    if (is_list_empty(doubly_linked_list)) {
        return TRUE;
    }

    current = get_list_head(doubly_linked_list);

    while (current != NULL) {
        next_node = current->next;
        if (current->data != NULL) {
            free_node_data(current->data);
        }
        free(current);
        current = next_node;
    }

    doubly_linked_list->data = NULL;
    doubly_linked_list->next = NULL;
    doubly_linked_list->prev = NULL;

    return TRUE;
}


int free_list(DoublyLinkedList** doubly_linked_list, void (*free_node_data)(void*)) {
    DoublyLinkedList* current = get_list_head(*doubly_linked_list);
    DoublyLinkedList* next_node;

    while (current != NULL) {
        next_node = current->next;
        if (current->data != NULL) {
            free_node_data(current->data);
        }
        free(current);
        current = next_node;
    }
    *doubly_linked_list = NULL;
    return TRUE;
}





