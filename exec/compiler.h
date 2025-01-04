#ifndef RAZ_AND_IDO_C_PROJECT_COMPILER_H
#define RAZ_AND_IDO_C_PROJECT_COMPILER_H
#include "../core/doubly_linked_list.h"
#include "../utils/commons.h"
#include "../utils/line_indexing.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../utils/logger.h"
#include "../core/symbol.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../core/operand.h"
#include "../utils/logger.h"
#include "../utils/commons.h"
#include "../core/doubly_linked_list.h"
#include "../utils/boolean.h"
#include "../core/encoded_line.h"
#include "../utils/globals.h"
#include "../core/entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




/**
 * Goes through the lines in line_list to perform the first pass,
 * updating the symbol_table and address_encoded_line_pair as needed.
 * updating the address_encoded_line_pair_list not excluding unresolved symbols and .Entry symbols
 * Returns a non-zero value if any issue is detected, 0 otherwise.
 */
int first_pass(DoublyLinkedList *line_list,
               DoublyLinkedList *symbol_table,
               DoublyLinkedList *address_encoded_line_pair_list,
               DoublyLinkedList *entry_list);

/**
 * Performs the second pass of processing based on the updated symbol_table,
 * address_encoded_line_pair_list, and entry_list.
 * Resolving unresolved symbols ent .Entry instructions
 *
 * Returns a non-zero value if any issue is detected, 0 otherwise.
 */
int second_pass(DoublyLinkedList *symbol_table,
                DoublyLinkedList *address_encoded_line_pair_list,
                DoublyLinkedList *entry_list);


#endif