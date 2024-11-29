#ifndef SYMBOL_H
#define SYMBOL_H
#include "doubly_linked_list.h"

typedef struct Symbol {
    char* label;
    unsigned long address;
    DoublyLinkedList* sym_properties;

}Symbol;

typedef enum {
    ENTRY_PROPERTY,
    EXTERNAL_PROPERTY,
    DATA_PROPERTY,
    CODE_PROPERTY
} SymbolProperty;

int add_symbol(DoublyLinkedList* symbol_table, char* label, unsigned long address, SymbolProperty property, int index);
Symbol* allocate_sym_mem(char* label, unsigned int address, DoublyLinkedList* sym_properties);

int free_symbol(Symbol* symbol);
int is_entry(Symbol symbol);
int is_external(Symbol symbol);





#endif