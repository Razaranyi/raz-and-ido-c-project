#ifndef OPERAND_H
#define OPERAND_H

#include "../core/symbol.h"
#include "../utils/boolean.h"
#include "../utils/logger.h"
#include "encoded_line.h"
#include "../utils/commons.h"
#include "address_encoded_pair.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


#define MAX_LABEL_LENGTH 31

/* Addressing Modes */
#define IMMEDIATE_ADDRESSING       0
#define DIRECT_ADDRESSING          1
#define RELATIVE_ADDRESSING        2
#define REGISTER_ADDRESSING        3
/*Struct to represent operand including all its vital data*/
typedef struct Operand {
    int index;                   /* Operand index (1 for source, 2 for destination) */
    int addressing_mode;         /* Addressing mode of the operand */
    char *operand_str;           /* Original operand string */
    int is_register;             /* TRUE if the operand is a register */
    int register_number;         /* Register number if operand is a register */
    int is_immediate;            /* TRUE if the operand is an immediate value */
    int immediate_value;         /* Immediate value if applicable */
    int is_symbol;               /* TRUE if the operand is a symbol (label) */
    char *symbol_name;           /* Symbol name if operand is a label */
} Operand;

/* Function Prototypes */
/**
 * Allocates memory for an Operand and returns a pointer to it.
 */
Operand* allocate_operand();

/**
 * Frees the memory occupied by the given Operand pointer.
 */
void free_operand(Operand *operand);

/**
 * Tries to get the register index from the provided raw operand string.
 * Returns a -1 if it's not recognized as a valid register.
 */
int get_register_index(char *raw_op);

/**
 * Figures out the addressing mode based on the operand string.
 * Returns an integer code for the mode according to addressing mode enum.
 */
int determine_addressing_mode(char *operand_str);

/**
 * Parse the operand string into an Operand structure.
 * index is the position in the instruction, line_index for error reporting.
 */
int parse_operand(char *operand_str, int index, Operand *operand, int line_index);



/**
 * Calculates the total extra words needed in memory for an array of Operands.
 * Also uses a DoublyLinkedList, the  IC, the line index and adding the encoded line.
 */
int count_extra_addresses_words(Operand operands[], int operand_count, DoublyLinkedList *address_encoded_line_pair_list, unsigned long IC, int line_index);

/**
 * Checks if the operand string is a valid string literal.
 * Returns non-zero if valid, zero if not.
 */
int is_valid_string(char *operand);

/**
 * Checks if the operand string is valid (e.g., register, label, or immediate).
 * Returns non-zero if valid, zero otherwise.
 */
int is_valid_operand(char *operand);

/**
 * Determines if the operand string represents a valid integer.
 * Returns non-zero if valid, zero if not.
 */
int is_valid_integer(char *operand);

/**
 * Parses the integer value from the operand string and stores it in out_value.
 * Returns non-zero if it succeeds, zero otherwise.
 */
int assign_valid_integer(char *operand, int *out_value);

#endif
