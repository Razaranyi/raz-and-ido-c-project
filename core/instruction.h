#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "doubly_linked_list.h"
#include "encoded_line.h"
#include "address_encoded_pair.h"
#include <stdio.h>
#include "operand.h"
#include "symbol.h"
#include "entry.h"


typedef enum {
    INVALID = 0,
    DATA = 1,
    STRING = 2,
    ENTRY = 3,
    EXTERN = 4
} Instruction;

/* Return enum of instruction // 0 when not an instruction */
Instruction get_instruction_enum(const char* instruction_name);


void parse_data_or_string_instruction(
        char *instruction_token,
        char *line_content,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found,
        int line_index
);


void process_instruction_line(
        char *line_content,
        Instruction instruction,
        char *label,
        int line_index,
        char *instruction_token,
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair,
        DoublyLinkedList *entry_list,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found
);

#endif
