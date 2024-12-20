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




int first_pass(DoublyLinkedList *line_list,
               DoublyLinkedList *symbol_table,
               DoublyLinkedList *address_encoded_line_pair,
               DoublyLinkedList *entry_list);




#endif