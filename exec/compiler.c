#include "compiler.h"
#include "../core/symbol.h"
#include "../core/instruction.h"
#include "../core/command.h"
#include "../core/operand.h"
#include "../utils/logger.h"
#include "../utils/commons.h"
#include "../core/doubly_linked_list.h"
#include "../utils/boolean.h"

void parse_extern_instruction(DoublyLinkedList *operands, DoublyLinkedList *symbol_table, int *error_found, int line_index);

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

    split_string_by_separator(operands_str, ",", operands, -1);


    /* Validate the number of operands */
    operand_count = get_list_length(*operands);


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
    current = get_list_head(*operands);
    while (current != NULL) {
        operand_str = (char *)current->data;
        if (!is_valid_operand(operand_str) && !is_string_begin_with_substring(command->command_name,"stop")) {
            errorf(line_index, "Invalid operand: '%s'", operand_str);
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
        unsigned long *IC,
        int *error_found
) {
    Command *command = find_command(command_token);
    char *operands_str;
    DoublyLinkedList *operands = NULL;
    if (command == NULL) {
        errorf(line_index, "Unknown command: '%s'", command_token);
        *error_found = TRUE;
        return;
    }

    operands_str = line_content + strlen(command_token);
    printf("operands_str: %s\n", operands_str);

    if (!isspace(*operands_str) && !is_string_begin_with_substring(command_token,"stop")) {
        errorf(line_index, "Missing space after command: '%s'", command_token);
        *error_found = TRUE;
        return;
    }

    remove_leading_and_trailing_whitespaces(operands_str, operands_str);
    parse_command_operands(command, operands_str, &operands, error_found, line_index);

    if (*error_found) {
        return;
    }

    if (label != NULL) {
        add_symbol(symbol_table, label, *IC, CODE_PROPERTY, line_index);
    }

    *IC += calculate_command_size(command, operands);
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
        DoublyLinkedList *current = get_list_head(operands);
        char *operand;
        while (current != NULL) {
            operand = (char *)current->data;
            if (!is_valid_integer(operand)) {
                errorf(line_index, "Invalid operand in .data instruction: '%s'", operand);
                *error_found = TRUE;
                return;
            }
            (*DC)++;
            current = current->next;
        }
    } else if (strcmp(instruction_token, ".string") == 0) {
        char *operand;
        if (get_list_length(operands) != 1) {
            errorf(line_index, ".string instruction expects a single string operand");
            *error_found = TRUE;
            return;
        }

        operand = (char *)operands->data;
        if (!is_valid_string(operand)) {
            errorf(line_index, "Invalid string in .string instruction: '%s'", operand);
            *error_found = TRUE;
            return;
        }

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
        if (label != NULL) {
            add_symbol(symbol_table, label, *DC, DATA_PROPERTY, line_index);
        }
        parse_data_or_string_instruction(
                instruction_token, operands, label, symbol_table, DC, error_found, line_index);
    } else if (instruction == EXTERN) {
        if (label != NULL) {
            warnf(line_index, "Label '%s' cannot be associated with '.extern' instruction", label);
        }
        parse_extern_instruction(operands, symbol_table, error_found, line_index);
    } else if (instruction == ENTRY) {
        if (label != NULL) {
            warnf(line_index, "Label '%s' cannot be associated with '.entry' instruction", label);
        }
    } else {
        errorf(line_index, "Unknown instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }
}

int first_pass(DoublyLinkedList *line_list, DoublyLinkedList *symbol_table) {
    unsigned long IC = 100;
    unsigned long DC = 0;
    int error_found = FALSE;
    printf("starting first pass...\n");

    DoublyLinkedList *current_node = get_list_head(line_list);
    while (current_node != NULL) {
        Line *line_entry = (Line *)current_node->data;
        char *line_content = line_entry->data;
        char *label = line_entry->label;
        int index = line_entry->index;
        char char1;
        debugf(index, "line content: %s",line_content);


        if (
            !is_string_begin_with_substring(line_content, ";") && line_content[0] != '\0') {
            debugf(index,"got in if");
            process_line(line_content, label, index, symbol_table, &IC, &DC, &error_found);
        }

        current_node = current_node->next;
    }

    return !error_found;
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
    char *line_copy = allocate_string(line_content);
    char *token;
    debugf(line_index,"starting processing line: %s",line_copy);

    split_string_by_separator(line_copy, " ", &tokens, -1);
    token = (char *)tokens->data;
    cut_spaces_start(token);


    debugf(line_index, "checking token: %s",token);

    if (find_command(token) != NULL) {
        debugf(line_index,"command found");
        process_command_line(line_content, label, line_index, token, symbol_table, IC, error_found);
    } else {
        Instruction instruction = get_instruction_enum(token);
        debugf(line_index,"Instruction: %d",instruction);

        if (instruction != INVALID) {
           debugf(line_index,"instruction found");

            process_instruction_line(
                    line_content, instruction, label, line_index, token, tokens->next, symbol_table, DC, error_found);
        } else {
            errorf(line_index, "Unrecognized command or instruction: '%s'", token);
            *error_found = TRUE;
        }
    }

    free_list(&tokens, free);
    free(line_copy);
}

void parse_extern_instruction(
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        int *error_found,
        int line_index
) {
    char *operand;
    if (get_list_length(operands) != 1) {
        errorf(line_index, ".extern instruction expects exactly one operand");
        *error_found = TRUE;
        return;
    }

    operand = (char *)operands->data;
    add_symbol(symbol_table, operand, 0, EXTERNAL_PROPERTY, line_index);
}

void mark_symbol_as_entry(
        DoublyLinkedList *symbol_table,
        char *symbol_name,
        int line_index
) {
    DoublyLinkedList *current = get_list_head(symbol_table);

    while (current != NULL) {
        Symbol *symbol = (Symbol *)current->data;
        if (strcmp(symbol->label, symbol_name) == 0) {
            if (symbol->sym_properties != ENTRY_PROPERTY) {
                symbol->sym_properties = ENTRY_PROPERTY;
            }
            return;
        }
        current = current->next;
    }

    warnf(line_index, "Symbol '%s' not found in the symbol table to mark as entry", symbol_name);
}
