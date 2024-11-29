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
        char *line_content,
        Instruction instruction,
        char *label,
        int line_index,
        char *command_token,
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        unsigned long *DC,
        int *error_found
);




//void process_command_line(
//        char *line_content,
//        char *label,
//        int line_index,
//        char *command_token,
//        DoublyLinkedList *symbol_table,
//        int *IC,
//        int *error_found
//);

void process_line(
        char *line_content,
        char *label,
        int line_index,
        DoublyLinkedList *symbol_table,
        unsigned long *IC,
        unsigned long *DC,
        int *error_found
);




#endif