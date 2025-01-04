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
        debugf(0,"command list is empty");
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
                "mov", 0, 0, 2,
                { TRUE, TRUE, FALSE, TRUE },   /* Allowed source: Immediate, Direct, Register */
                { FALSE, TRUE, FALSE, TRUE }   /* Allowed destination: Direct, Register */
        },
        /* cmp  */
        {
                "cmp", 1, 0, 2,
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
                "lea", 4, 0, 2,
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
                "red", 12, 0, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, TRUE, FALSE, TRUE }    /* Allowed destination: Direct, Register */
        },
        /* prn */
        {
                "prn", 13, 0, 1,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { TRUE, TRUE, FALSE, TRUE }     /* Allowed destination: Immediate, Direct, Register */
        },
        /* trs  */
        {
                "rts", 14, 0, 0,
                { FALSE, FALSE, FALSE, FALSE }, /* No source operand */
                { FALSE, FALSE, FALSE, FALSE }  /* No destination operand */
        },
        /* stop */
        {
                "stop", 15, 0, 0,
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
            errorf(-1,"Aloocation failed");
            exit(1);
        }

        command->command_name = allocate_string(command_init_data[i].command_name);
        if (command->command_name == NULL) {
            free(command);
            errorf(-1,"Command not found");
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

/* Checks commands operands & Calculates the size (in words) of a command */
int handle_command_operands(Command *command,
                            DoublyLinkedList *operands,
                            DoublyLinkedList  *address_encoded_line_pair_list,
                            EncodedLine *encoded_line,
                            int line_index,
                            int *error_found,
                            unsigned long *IC
                            ) {
    Operand *operand_array = calloc(command->number_of_operands, sizeof(Operand));
    int operand_count = 0;
    int extra_words = 0;
    int i;
    int total_operands;

    DoublyLinkedList *current;
    AddressEncodedPair *address_encoded_pair;

    if (!operand_array) {
        errorf(-1,"Memory allocation failed for operand array");
        exit(1);
    }

    current = get_list_head(operands);
    while (current != NULL && operand_count < command->number_of_operands) {
        parse_operand((char *)current->data, operand_count, &operand_array[operand_count], line_index);
        if (operand_array[operand_count].is_register){
            if (command->number_of_operands == 1){
                encoded_line_set_reg(encoded_line,
                                     operand_array[operand_count].register_number, TRUE);
            } else
                encoded_line_set_reg(encoded_line,operand_array[operand_count].register_number,operand_count);
        }
        operand_count++;
        current = current->next;
    }

    total_operands = command->number_of_operands;

    if (total_operands == 0) {
        encoded_line_set_src_addressing(encoded_line,0);
        encoded_line_set_dst_addressing(encoded_line,0);

    } else if (total_operands == 1) {
        debugf(line_index,"checking operand: %s addressing mode: %d",
               operand_array[0].operand_str,
               operand_array[0].addressing_mode);
        /* The single operand is a destination operand */
        if (!is_dst_addressing_mode_allowed(command, operand_array[0].addressing_mode)) {
            errorf(line_index, "Invalid addressing mode for command '%s', operand '%s'",
                   command->command_name, operand_array[0].operand_str);
            *error_found = TRUE;
        }
        encoded_line_set_dst_addressing(encoded_line, operand_array[0].addressing_mode);
        encoded_line_set_src_addressing(encoded_line,0);
    } else if (total_operands == 2) {
        debugf(line_index,"checking operands: %s, %s addressing modes: %d, %d",
               operand_array[0].operand_str,
               operand_array[1].operand_str,
               operand_array[0].addressing_mode,
               operand_array[1].addressing_mode);
        /* First operand is source, second is destination */
        if (!is_src_addressing_mode_allowed(command, operand_array[0].addressing_mode)) {
            errorf(line_index, "Invalid source addressing mode for command '%s', operand '%s'",
                   command->command_name, operand_array[0].operand_str);
            *error_found = TRUE;
        }
        if (!is_dst_addressing_mode_allowed(command, operand_array[1].addressing_mode)) {
            errorf(line_index, "Invalid destination addressing mode for command '%s', operand '%s'",
                   command->command_name, operand_array[1].operand_str);
            *error_found = TRUE;
        }
        encoded_line_set_src_addressing(encoded_line, operand_array[0].addressing_mode);
        encoded_line_set_dst_addressing(encoded_line, operand_array[1].addressing_mode);
    }
    address_encoded_pair = create_address_encoded_pair(*IC,encoded_line);
    add_to_list(address_encoded_line_pair_list,address_encoded_pair);


    debugf(line_index,"Inserted addressing modes. src: %lu, dest: %lu ",encoded_line->src_addressing,encoded_line->dst_addressing);

    extra_words = count_extra_addresses_words(operand_array, operand_count,address_encoded_line_pair_list,*IC,line_index);

    for (i=0;i<command->number_of_operands;i++){
        free(operand_array[i].operand_str);
        free(operand_array[i].symbol_name);
    }
    free(operand_array);
    return 1 + extra_words; /* Command word + extra words */
}


void parse_command_operands(
        Command *command,
        char *operands_str,
        DoublyLinkedList **operands,
        int *error_found,
        int line_index
) {
    char *operand_str;
    DoublyLinkedList *current;
    int operand_count;

    if (operands_str[0] == ','){
        errorf(line_index,"Illegal comma after command: %s",command->command_name);
        *error_found = TRUE;
    }

    /* Handle the case where no operands are expected */
    if (command->number_of_operands == 0) {
        if (strlen(operands_str) > 0) {
            errorf(
                    line_index,
                    "Command '%s' expects 0 operands but got operand '%s'",
                    command->command_name,
                    operands_str
            );
            *error_found = TRUE;
        }
        return;
    }

    /* Split operands by ',' */
    split_string_by_separator(operands_str, ",", operands, -1);

    /* Validate the number of operands */
    operand_count = get_list_length(*operands);
    if (operand_count != command->number_of_operands) {
        current = get_list_tail(*operands);
        operand_str = current->data;
        if (operand_str[0] == '\0'){
            errorf(line_index, "Extra comma found at the end of text");
        } else{
            errorf(
                    line_index,
                    "Command '%s' expects %d operands but got %d",
                    command->command_name,
                    command->number_of_operands,
                    operand_count
            );
        }

        *error_found = TRUE;
        free_list(operands, free);
        return;
    }

    /* Validate each operand */
    current = get_list_head(*operands);
    while (current != NULL) {
        operand_str = (char *)current->data;
        remove_leading_and_trailing_whitespaces(operand_str,operand_str);
        if (!is_valid_operand(operand_str)) {
            errorf(line_index, "Missing comma or invalid operand: '%s'", operand_str);
            *error_found = TRUE;
            free_list(operands, free);
            return;
        }
        current = current->next;
    }
}

void process_command_line(
        char *line_content,
        char *label,
        int line_index,
        char *command_token,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair_list,
        unsigned long *IC,
        int *error_found
) {
    Command *command = find_command(command_token);
    char *operands_str;
    EncodedLine *encodedLine = create_encoded_line();
    DoublyLinkedList *operands = NULL;

    debugf(line_index,"Assigning command opt and funct. Command: %s, opscode: %d, funct: %d ",command->command_name,command->opcode,command->funct);

    encoded_line_set_opcode(encodedLine,command->opcode);
    encoded_line_set_funct(encodedLine,command->funct);
    encoded_line_set_are(encodedLine,A);



    operands_str = line_content + strlen(command_token);

    /* Remove leading and trailing spaces from operands_str */
    remove_leading_and_trailing_whitespaces(operands_str, operands_str);

    parse_command_operands(command, operands_str, &operands, error_found, line_index);

    if (*error_found) {
        return;
    }

    /* Add the label to the symbol table if it exists */
    if (strcmp(label,"")!=0) {
        add_symbol(symbol_table, label, *IC,0, CODE_PROPERTY, line_index);
    }

    /* Increment IC based on the size of the command and operands */
    *IC += handle_command_operands(command, operands,address_encoded_line_pair_list, encodedLine, line_index,error_found,IC);
    free_list(&operands, free);
}
