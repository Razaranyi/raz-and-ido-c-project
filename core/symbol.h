#ifndef SYMBOL_H
#define SYMBOL_H
#include "doubly_linked_list.h"
#include "../utils/boolean.h"
#include "../utils/commons.h"
#include <stdlib.h>
#include <string.h>

typedef struct Symbol {
    char* label;
    unsigned long address;
    unsigned long value;
    DoublyLinkedList* sym_properties;
    DoublyLinkedList* external_usages;

}Symbol;

typedef enum {
    ENTRY_PROPERTY,
    EXTERNAL_PROPERTY,
    DATA_PROPERTY,
    CODE_PROPERTY
} SymbolProperty;

int add_symbol(DoublyLinkedList* symbol_table, char* label, unsigned long address,unsigned long value,SymbolProperty property, int index);
Symbol* allocate_sym_mem(char* label, unsigned int address,unsigned long value, DoublyLinkedList* sym_properties);
int add_external_usage(Symbol* symbol, unsigned long usage_address);
void free_symbol_data(void* data);
int free_symbol(Symbol* symbol);
int is_entry(Symbol symbol);
int is_external(Symbol symbol);
int is_property(Symbol symbol, SymbolProperty property);
int symbols_table_get_symbol(DoublyLinkedList *symbol_table, char *name, Symbol **symbol);
int mark_symbol_as_entry(DoublyLinkedList *symbol_table, char *symbol_name, int line_index);





#endif