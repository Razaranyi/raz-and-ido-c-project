#ifndef SYMBOL_H
#define SYMBOL_H

#include "doubly_linked_list.h"
#include "../utils/boolean.h"
#include "../utils/commons.h"
#include <stdlib.h>
#include <string.h>
#include "entry.h"

/**
 * Represents a symbol with a label, address, value, and related properties.
 * sym_properties and external_usages are stored in DoublyLinkedLists.
 */
typedef struct Symbol {
    char* label;
    unsigned long address;
    unsigned long value;
    DoublyLinkedList* sym_properties;
    DoublyLinkedList* external_usages;
} Symbol;

/**
 * Lists out the types of symbol properties that can be assigned to a Symbol.
 */
typedef enum {
    ENTRY_PROPERTY,
    EXTERNAL_PROPERTY,
    DATA_PROPERTY,
    CODE_PROPERTY
} SymbolProperty;

/**
 * Adds a new symbol to the symbol table with the given label, address, value, and property.
 * index will be used for error reporting.
 */
int add_symbol(DoublyLinkedList* symbol_table, char* label, unsigned long address,unsigned long value, SymbolProperty property, int index);

/**
 * Allocates memory for a Symbol structure and initializes it with the given parameters.
 */
Symbol* allocate_sym_mem(char* label, unsigned int address, unsigned long value, DoublyLinkedList* sym_properties);

/**
 * Adds a record of an external usage to the given Symbol.
 * usage_address is where this symbol is used internally.
 */
int add_external_usage(Symbol* symbol, unsigned long usage_address);

/**
 * Wrapper for free symbol to be used as a function pointer in free_list
 */
void free_symbol_data(void* data);

/**
 * Cleans up a Symbol object, freeing any allocated fields inside.
 * Returns an integer status code if needed.
 */
int free_symbol(Symbol* symbol);

/**
 * Checks if the given Symbol has the ENTRY_PROPERTY.
 * Returns non-zero if yes, zero otherwise.
 */
int is_entry(Symbol symbol);

/**
 * Checks if the given Symbol has the EXTERNAL_PROPERTY.
 * Returns non-zero if yes, zero otherwise.
 */
int is_external(Symbol symbol);

/**
 * Checks if the given Symbol has a certain property.
 * Returns non-zero if it does, zero otherwise.
 */
int is_property(Symbol symbol, SymbolProperty property);

/**
 * Looks up a Symbol by name in the symbol table.
 * If found, stores a pointer to it in 'symbol' and returns non-zero, else returns zero.
 */
int symbols_table_get_symbol(DoublyLinkedList *symbol_table, char *name, Symbol **symbol);

/**
 * Marks a Symbol as an entry in the symbol table using the symbol_name.
 * If it's not found or an error occurs, use line_index for reporting.
 */
int mark_symbol_as_entry(DoublyLinkedList *symbol_table, char *symbol_name, int line_index);

/**
 * Loops through an entry list, finds matching symbols in the symbol table, and marks them as entries.
 * If any issue arises, sets error_found to TRUE.
 */
void mark_entries(DoublyLinkedList *entry_list, DoublyLinkedList *symbol_table, int *error_found);

#endif
