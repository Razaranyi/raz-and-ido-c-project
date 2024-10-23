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
    while (current->prev != NULL){
        current = current->prev;
    }
    return current;
}

DoublyLinkedList* get_list_tail(DoublyLinkedList* doublyLinkedList){
    DoublyLinkedList* current = doublyLinkedList;
    while(current->next != NULL){
        current = current.next;
    }
    return current;
}

int get_list_length(DoublyLinkedList* doublyLinkedList){
    int length = 0;
    DoublyLinkedList* current = get_list_head(doublyLinkedList);

    if(current->data != NULL){
        length+=1;
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
    tail->next = new_node;

    return TRUE;
}

int merge_lists(DoublyLinkedList* doubly_linked_list1, DoublyLinkedList* doubly_linked_list2) {
    DoublyLinkedList* current = get_list_head(doubly_linked_list2);
    while (current != NULL) {
        add_to_list(doubly_linked_list1, current->data);
        current = current->next;
    }
    return TRUE;
}


int is_list_empty(DoublyLinkedList* doubly_linked_list){
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

int free_list(DoublyLinkedList* doubly_linked_list, void (*free_node_data)(void*)) {
    if (is_list_empty(doubly_linked_list)) {
        return TRUE;
    }

    DoublyLinkedList* current = get_list_head(doubly_linked_list);
    DoublyLinkedList* next_node;

    while (current != NULL) {
        next_node = current->next;
        if (current->data != NULL) {
            free_node_data(current->data);
        }
        free(current);
        current = next_node;
    }
    return TRUE;
}





