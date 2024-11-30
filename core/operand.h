#ifndef OPERAND_H
#define OPERAND_H

#include "../core/symbol.h"
#include "../utils/boolean.h"
#include "../utils/logger.h"


#define MAX_LABEL_LENGTH 31

/* Addressing Modes */
#define IMMEDIATE_ADDRESSING       0
#define DIRECT_ADDRESSING          1
#define RELATIVE_ADDRESSING        2
#define REGISTER_ADDRESSING        3

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
Operand* allocate_operand();
void free_operand(Operand *operand);
int get_register_index(char *raw_op);
int determine_addressing_mode(char *operand_str);
int parse_operand(char *operand_str, int index, Operand *operand, int line_index, char *error_message);
int count_extra_address_words(Operand *operand);
int count_extra_addresses_words(Operand operands[], int operand_count);

#endif
