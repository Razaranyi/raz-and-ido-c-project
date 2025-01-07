#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "doubly_linked_list.h"
#include "encoded_line.h"
#include "address_encoded_pair.h"
#include <stdio.h>
#include "symbol.h"
#include "entry.h"
#include "../core/operand.h"

/*Enum to represent types of instruction commands and invalid case*/
typedef enum {
    INVALID = 0,
    DATA = 1,
    STRING = 2,
    ENTRY = 3,
    EXTERN = 4
} Instruction;

/* Return enum of instruction or 0 when not an instruction */
Instruction get_instruction_enum(char* instruction_name);


/*Extract type of data/string instruction and handle operands and calculate binary address accordingly*/
void parse_data_or_string_instruction(
        char *instruction_token,
        char *line_content,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair_list,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found,
        int line_index
);

/*Extract instruction type in a line and send it to the appropriate function for additional processing*/
void process_instruction_line(
        char *line_content,
        Instruction instruction,
        char *label,
        int line_index,
        char *instruction_token,
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair_list,
        DoublyLinkedList *entry_list,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found
);

#endif
