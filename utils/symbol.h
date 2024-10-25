#ifndef SYMBOL_H
#define SYMBOL_H
#include "doubly_linked_list.h"

typedef struct Symbol {
    char* label;
    int value;
    DoublyLinkedList* sym_addresses;
    DoublyLinkedList* sym_properties;

}Symbol;

typedef enum {
    ENTRY,
    EXTERNAL
} SymbolProperty;


Symbol* allocate_sym_mem(char* label, int value, DoublyLinkedList* sym_properties);
int free_symbol(Symbol* symbol);
int is_entry(Symbol symbol);
int is_external(Symbol symbol);




#endif