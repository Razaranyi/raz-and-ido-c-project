#include "compiler.h"
#include "../core/symbol.h"

void parse_command_operands(
        Command *command,
        char *operands_str,
        DoublyLinkedList **operands,
        int *error_found,
        int line_index
) {
    /* Split operands by ',' */
    split_string_by_separator(operands_str, ",", operands, -1);

    /* Validate the number of operands */
    int operand_count = get_list_length(*operands);
    if (operand_count != command->number_of_operands) {
        errorf(
                line_index,
                "Command '%s' expects %d operands but got %d",
                command->command_name,
                command->number_of_operands,
                operand_count
        );
        *error_found = TRUE;
        free_list(operands, free);
        return;
    }

    /* Validate each operand */
    DoublyLinkedList *current = get_list_head(*operands);
    while (current != NULL) {
        char *operand = (char *)current->data;

        /* Add validation logic for operands if necessary */
        if (!is_valid_operand(operand)) { /* Example: implement `is_valid_operand` */
            errorf(line_index, "Invalid operand: '%s'", operand);
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
        int *IC,
        int *error_found
) {
    Command *command = find_command(command_token);
    if (command == NULL) {
        errorf(line_index, "Unknown command: '%s'", command_token);
        *error_found = TRUE;
        return;
    }

    /* Skip the command token and find the start of the operands */
    char *operands_str = line_content + strlen(command_token);

    /* Ensure there's at least one space after the command token */
    if (!isspace(*operands_str)) {
        errorf(line_index, "Missing space after command: '%s'", command_token);
        *error_found = TRUE;
        return;
    }

    remove_leading_and_trailing_whitespaces(operands_str,operands_str);

    /* Parse operands */
    DoublyLinkedList *operands = NULL;
    parse_command_operands(command, operands_str, &operands, error_found, line_index);

    if (*error_found) {
        return; /* Error already reported in parse_command_operands */
    }

    /* Increment IC based on the size of the command and operands */
    *IC += calculate_command_size(command, operands);

    /* Free the operand list */
    free_list(&operands, free);
}




void parse_data_or_string_instruction(
        char *instruction_token,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        unsigned long *DC,
        int *error_found,
        int line_index
) {
    if (strcmp(instruction_token, ".data") == 0) {
        /* Process .data instruction */
        DoublyLinkedList *current = get_list_head(operands);
        while (current != NULL) {
            char *operand = (char *)current->data;

            /* Validate operand as an integer */
            if (!is_valid_integer(operand)) {
                errorf(line_index, "Invalid operand in .data instruction: '%s'", operand);
                *error_found = TRUE;
                return;
            }

            /* Increment Data Counter for each operand */
            (*DC)++;
            current = current->next;
        }



    } else if (strcmp(instruction_token, ".string") == 0) {
        /* Validate single operand as a string */
        if (get_list_length(operands) != 1) {
            errorf(line_index, ".string instruction expects a single string operand");
            *error_found = TRUE;
            return;
        }

        char *operand = (char *)operands->data;
        if (!is_valid_string(operand)) {
            errorf(line_index, "Invalid string in .string instruction: '%s'", operand);
            *error_found = TRUE;
            return;
        }

        /* Increment Data Counter for each character (including null terminator) */
        (*DC) += strlen(operand) + 1;

    } else {
        errorf(line_index, "Unexpected instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }
}





void process_instruction_line(
        char *line_content,
        Instruction instruction,
        char *label,
        int line_index,
        char *instruction_token,
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        unsigned long *DC,
        int *error_found
) {
    if (instruction == DATA || instruction == STRING) {
        /* If there's a label, add it to the symbol table as a DATA_SYMBOL */
        if (label != NULL) {
            add_symbol(symbol_table, label, *DC, DATA_PROPERTY,line_index);
        }

        /* Parse the data or string instruction */
        parse_data_or_string_instruction(
                instruction_token,
                operands,
                label,
                symbol_table,
                DC,
                error_found,
                line_index
        );
    } else if (instruction == EXTERN) {
        if (label != NULL) {
            warnf(line_index, "Label '%s' cannot be associated with '.extern' instruction", label);
        }
        parse_extern_instruction(operands, symbol_table, error_found, line_index);
    } else if (instruction == ENTRY) {
        if (label != NULL) {
            warnf(line_index, "Label '%s' cannot be associated with '.entry' instruction", label);
        }
        /* .entry is handled in the second pass */
    } else {
        errorf(line_index, "Unknown instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }
}




/* Main first pass function */
int first_pass(DoublyLinkedList *line_list, DoublyLinkedList *symbol_table) {
    unsigned long IC = 100;
    unsigned long DC = 0;
    int error_found = FALSE;

    /* Iterate through each line in the list */
    DoublyLinkedList *current_node = get_list_head(line_list);
    while (current_node != NULL) {
        Line *line_entry = (Line *)current_node->data;
        char *line_content = line_entry->data;
        char *label = line_entry->label;
        int index = line_entry->index;

        /* Skip empty or comment lines */
        if (!is_string_begin_with_substring(line_content, "\0") &&
            !is_string_begin_with_substring(line_content, ";")) {

            /* Process the current line */
            process_line(line_content, label,index, symbol_table, &IC, &DC, &error_found);
        }

        current_node = current_node->next;
    }

    return !error_found; /* Return TRUE if no errors were found */
}

void process_line(
        char *line_content,
        char *label,
        int line_index,
        DoublyLinkedList *symbol_table,
        unsigned long *IC,
        unsigned long *DC,
        int *error_found
) {
    DoublyLinkedList *tokens = NULL;
    char *line_copy = allocate_string(line_content); /* Create a mutable copy */

    /* Tokenize the line */
    split_string_by_separator(line_copy, " \t", &tokens, -1);

    /* Get the first token (command or instruction) */
    char *token = (char *)tokens->data;

    /* Process based on line type */
    if (find_command(token) != NULL) {
        /* Command line (assembly command like 'mov', 'add') */
        process_command_line(
                line_content,
                label,
                line_index,
                token,
                symbol_table,
                IC,
                error_found
        );
    } else {
        Instruction instruction = get_instruction_enum(token);
        if (instruction != INVALID) {
            /* Instruction line (instructions like '.data', '.string') */
            process_instruction_line(
                    line_content,
                    instruction,
                    label,
                    line_index,
                    token,
                    tokens->next,
                    symbol_table,
                    DC,
                    error_found
            );
        } else {
            /* Unrecognized line */
            errorf(line_index, "Unrecognized command or instruction: '%s'", token);
            *error_found = TRUE;
        }
    }

    /* Free resources */
    free_list(&tokens, free);
    free(line_copy);
}




