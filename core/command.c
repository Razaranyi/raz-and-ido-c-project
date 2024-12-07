#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "../utils/commons.h"
#include "../core/operand.h"
#include "doubly_linked_list.h"

/* Global command list */
DoublyLinkedList* command_list = NULL;

/* Function to free the data stored in the command list nodes */
static void free_command_data(void* data) {
    Command* command = (Command*)data;
    if (command != NULL) {
        if (command->command_name != NULL) {
            free(command->command_name);
        }
        free(command);
    }
}

/* Frees the command set */
void free_command_set() {
    free_list(&command_list, free_command_data);
}

int is_command_name(char* name) {
    Command* command = find_command(name);
    return (command != NULL) ? TRUE : FALSE;
}

/* Finds an command by name */
Command* find_command(char* name) {
    DoublyLinkedList* current;
    Command* command;

    if (command_list == NULL || is_list_empty(command_list)) {
        debugf(-1,"command list is empty");
        return NULL;
    }

    current = get_list_head(command_list);

    while (current != NULL) {
        command = (Command*)current->data;
        if (strcmp(command->command_name, name) == 0) {
            return command;
        }
        current = current->next;
    }

    return NULL;
}

/* Checks if a given addressing mode is allowed for the source operand */
int is_src_addressing_mode_allowed(Command* command, int mode) {
    if (command == NULL || mode < 0 || mode > 3) {
        return FALSE;
    }
    return command->allowed_src_addressing_modes[mode];
}

/* Checks if a given addressing mode is allowed for the destination operand */
int is_dst_addressing_mode_allowed(Command* command, int mode) {
    if (command == NULL || mode < 0 || mode > 3) {
        return FALSE;
    }
    return command->allowed_dst_addressing_modes[mode];
}

static const Command command_init_data[] = {
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

void initialize_command_set() {
    int num_commands = sizeof(command_init_data) / sizeof(command_init_data[0]);
    int i, j;
    Command *command;



    command_list = allocate_node_mem();

    for (i = 0; i < num_commands; i++) {
        command = (Command *) malloc(sizeof(Command));
        if (command == NULL) {
            error("Aloocation failed",__LINE__);
            exit(1);
        }

        command->command_name = allocate_string(command_init_data[i].command_name);
        if (command->command_name == NULL) {
            free(command);
            error("Command not found",__LINE__);
            exit(1);
        }

        command->opcode = command_init_data[i].opcode;
        command->funct = command_init_data[i].funct;
        command->number_of_operands = command_init_data[i].number_of_operands;

        /* Copy allowed source addressing modes */
        for (j = 0; j < 4; j++) {
            command->allowed_src_addressing_modes[j] = command_init_data[i].allowed_src_addressing_modes[j];
        }

        /* Copy allowed destination addressing modes */
        for (j = 0; j < 4; j++) {
            command->allowed_dst_addressing_modes[j] = command_init_data[i].allowed_dst_addressing_modes[j];
        }
        add_to_list(command_list, command);

    }
}

/* Calculates the size (in words) of a command */
int calculate_command_size(Command *command, DoublyLinkedList *operands) {
    Operand *operand_array = malloc(sizeof(Operand) * command->number_of_operands);
    int operand_count = 0;
    int extra_words = 0;
    DoublyLinkedList *current;

    if (!operand_array) {
        error("Memory allocation failed for operand array", __LINE__);
        return -1;
    }

    current = get_list_head(operands);
    while (current != NULL && operand_count < command->number_of_operands) {
        parse_operand((char *)current->data, operand_count, &operand_array[operand_count], 0, NULL);
        operand_count++;
        current = current->next;
    }

    extra_words = count_extra_addresses_words(operand_array, operand_count);

    free(operand_array);
    return 1 + extra_words; /* Command word + extra words */
}

