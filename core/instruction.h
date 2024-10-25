#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../utils/boolean.h"
#include "doubly_linked_list.h"

/* addressing modes */
#define ADDRESSING_IMMEDIATE 0
#define ADDRESSING_DIRECT    1
#define ADDRESSING_RELATIVE  2
#define ADDRESSING_REGISTER  3

/* Instruction structure */
typedef struct {
    char* instruction_name;
    int opcode;
    int funct;  /* Set to -1 when not used */
    int number_of_operands;

    /* Allowed addressing modes for source and destination operands */
    int allowed_src_addressing_modes[4];
    int allowed_dst_addressing_modes[4];
} Instruction;

/* Global instruction set */
extern DoublyLinkedList* instruction_list;


/* Initializes the instruction set */
void initialize_instruction_set();

/* free instruction set */
void free_instruction_set();

/* Finds an instruction by name */
Instruction* find_instruction(char* name);

/* Cheks if given name is an instruction name */
int is_instruction_name(char* name);

/* Checks if a given addressing mode is allowed for the source operand */
int is_src_addressing_mode_allowed(Instruction* instr, int mode);

/* Checks if a given addressing mode is allowed for the destination operand */
int is_dst_addressing_mode_allowed(Instruction* instr, int mode);



#endif
