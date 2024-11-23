#ifndef RAZ_AND_IDO_C_PROJECT_COMPILER_H
#define RAZ_AND_IDO_C_PROJECT_COMPILER_H
#include "../core/doubly_linked_list.h"
#include "../utils/commons.h"
#include "../utils/line_indexing.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../utils/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Function prototypes */
void process_instruction_line(
        char *instruction_token,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        int *IC,
        int *error_found,
        int line_number
);

void process_command_line(
        char *command_token,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        int *DC,
        int *error_found,
        int line_number
);

void process_line(
        char *line_content,
        int line_number,
        DoublyLinkedList *symbol_table,
        int *IC,
        int *DC,
        int *error_found
);


#endif