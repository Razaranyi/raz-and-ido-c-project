#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "../utils/commons.h"
#include "../utils/boolean.h"
#include "doubly_linked_list.h"

/* Global instruction list */
DoublyLinkedList* instruction_list = NULL;

/* Function to free the data stored in the instruction list nodes */
static void free_instruction_data(void* data) {
    Instruction* instr = (Instruction*)data;
    if (instr != NULL) {
        if (instr->instruction_name != NULL) {
            free(instr->instruction_name);
        }
        free(instr);
    }
}

/* Frees the instruction set */
void free_instruction_set() {
    free_list(&instruction_list, free_instruction_data);
}

int is_instruction_name(char* name) {
    Instruction* instr = find_instruction(name);
    return (instr != NULL) ? TRUE : FALSE;
}

/* Finds an instruction by name */
Instruction* find_instruction(char* name) {
    DoublyLinkedList* current;
    Instruction* instr;

    if (instruction_list == NULL || is_list_empty(instruction_list)) {
        return NULL;
    }

    current = get_list_head(instruction_list);

    while (current != NULL) {
        instr = (Instruction*)current->data;
        if (strcmp(instr->instruction_name, name) == 0) {
            return instr;
        }
        current = current->next;
    }

    return NULL;
}

/* Checks if a given addressing mode is allowed for the source operand */
int is_src_addressing_mode_allowed(Instruction* instr, int mode) {
    if (instr == NULL || mode < 0 || mode > 3) {
        return FALSE;
    }
    return instr->allowed_src_addressing_modes[mode];
}

/* Checks if a given addressing mode is allowed for the destination operand */
int is_dst_addressing_mode_allowed(Instruction* instr, int mode) {
    if (instr == NULL || mode < 0 || mode > 3) {
        return FALSE;
    }
    return instr->allowed_dst_addressing_modes[mode];
}

static const Instruction instruction_init_data[] = {
        /* mov */
        {
                "mov", 0, -1, 2,
                { TRUE, TRUE, FALSE, TRUE },   /* Allowed source: Immediate, Direct, Register */
                { FALSE, TRUE, FALSE, TRUE }   /* Allowed destination: Direct, Register */
        },
        /* cmp  */
        {
                "cmp", 1, -1, 2,
                { TRUE, TRUE, FALSE, TRUE },   /* Allowed source: Immediate, Direct, Register */
                { TRUE, TRUE, FALSE, TRUE }    /* Allowed destination: Immediate, Direct, Register */
        },
        /* add */
        {
                "add", 2, 1, 2,
                { TRUE, TRUE, FALSE, TRUE },   /* Allowed source: Immediate, Direct, Register */
                { FALSE, TRUE, FALSE, TRUE }   /* Allowed destination: Direct, Register */
        },
        /* sub */
        {
                "sub", 2, 2, 2,
                { TRUE, TRUE, FALSE, TRUE },   /* Allowed source: Immediate, Direct, Register */
                { FALSE, TRUE, FALSE, TRUE }   /* Allowed destination: Direct, Register */
        },
        /* lea */
        {
                "lea", 4, -1, 2,
                { FALSE, TRUE, FALSE, FALSE }, /* Allowed source: Direct */
                { FALSE, TRUE, FALSE, TRUE }   /* Allowed destination: Direct, Register */
        },
        /* clr */
        {
                "clr", 5, 1, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* not */
        {
                "not", 5, 2, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* inc */
        {
                "inc", 5, 3, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* dec */
        {
                "dec", 5, 4, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* jmp */
        {
                "jmp", 9, 1, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, TRUE, FALSE }    /* Allowed destination: Direct, Relative */
        },
        /* bne  */
        {
                "bne", 9, 2, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, TRUE, FALSE }    /* Allowed destination: Direct, Relative */
        },
        /* jsr */
        {
                "jsr", 9, 3, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, TRUE, FALSE }    /* Allowed destination: Direct, Relative */
        },
        /* red */
        {
                "red", 12, -1, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* prn */
        {
                "prn", 13, -1, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { TRUE, TRUE, FALSE, TRUE }     /* Allowed destination: Immediate, Direct, Register */
        },
        /* trs  */
        {
                "rts", 14, -1, 0,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, FALSE, FALSE, FALSE }  /* No destination operand */
        },
        /* stop */
        {
                "stop", 15, -1, 0,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, FALSE, FALSE, FALSE }  /* No destination operand */
        }
};

void initialize_instruction_set() {
    int num_instructions = sizeof(instruction_init_data) / sizeof(instruction_init_data[0]);
    int i, j;
    Instruction *instr;

    instruction_list = allocate_node_mem();

    for (i = 0; i < num_instructions; i++) {
        instr = (Instruction *) malloc(sizeof(Instruction));
        if (instr == NULL) {
            error("Aloocation failed",__LINE__);
            exit(1);
        }

        instr->instruction_name = allocate_string(instruction_init_data[i].instruction_name);
        if (instr->instruction_name == NULL) {
            free(instr);
            error("Instruction not found",__LINE__);
            exit(1);
        }

        instr->opcode = instruction_init_data[i].opcode;
        instr->funct = instruction_init_data[i].funct;
        instr->number_of_operands = instruction_init_data[i].number_of_operands;

        /* Copy allowed source addressing modes */
        for (j = 0; j < 4; j++) {
            instr->allowed_src_addressing_modes[j] = instruction_init_data[i].allowed_src_addressing_modes[j];
        }

        /* Copy allowed destination addressing modes */
        for (j = 0; j < 4; j++) {
            instr->allowed_dst_addressing_modes[j] = instruction_init_data[i].allowed_dst_addressing_modes[j];
        }

        add_to_list(instruction_list, instr);
    }
}
