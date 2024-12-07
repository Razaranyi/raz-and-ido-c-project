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

void check_double_commas(int line_index, char *line_copy, int *error_found) {
    int last_was_comma = 0;
    int i;

    for (i = 0; line_copy[i] != '\0'; i++) {
        char current_char = line_copy[i];

        if (current_char == ',') {
            if (last_was_comma) {
                errorf(line_index, "Multiple consecutive commas");
                *error_found = TRUE;
                return;
            }
            last_was_comma = 1;
        } else if (!isspace(current_char)) {
            last_was_comma = 0;
        }
    }
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

    remove_leading_and_trailing_whitespaces(operands_str, operands_str);
    if (operands_str[0] == ','){
        errorf(line_index,"Illegal comma after command: %s",command->command_name);
        *error_found = TRUE;
    }

    /* Handle the case where no operands are expected */
    if (command->number_of_operands == 0) {
        if (strlen(operands_str) > 0) {
            errorf(
                    line_index,
                    "Command '%s' expects 0 operands but got '%s'",
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
        printf("operand: %s\n",operand_str);
        if (!is_valid_operand(operand_str)) {
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

    /* Ensure there's at least one space after the command token if operands are expected */
    if (command->number_of_operands > 0 && !isspace(*operands_str)) {
        errorf(line_index, "Missing space after command: '%s'", command_token);
        *error_found = TRUE;
        return;
    }

    /* Remove leading and trailing spaces from operands_str */
    remove_leading_and_trailing_whitespaces(operands_str, operands_str);

    /* Parse operands */
    parse_command_operands(command, operands_str, &operands, error_found, line_index);

    if (*error_found) {
        return;
    }

    /* Add the label to the symbol table if it exists */
    if (strcmp(label,"")!=0) {
        add_symbol(symbol_table, label, *IC,0, CODE_PROPERTY, line_index);
    }

    /* Increment IC based on the size of the command and operands */
    *IC += calculate_command_size(command, operands);
    free_list(&operands, free);
}



void parse_data_or_string_instruction(
        char *instruction_token,
        char *line_content,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found,
        int line_index
) {
    char *line_copy = allocate_string(line_content);
    char *after_instruction;
    DoublyLinkedList *current;
    char *operand;
    DoublyLinkedList *operands_list = NULL;

    after_instruction = line_copy + strlen(instruction_token);

    remove_leading_and_trailing_whitespaces(after_instruction, after_instruction);
    if (after_instruction[0] == ','){
        errorf(line_index,"Illegal comma after instruction: %s",instruction_token);
        *error_found = TRUE;
    }



    split_string_by_separator(after_instruction, ",", &operands_list, -1);

    current = get_list_tail(operands_list);
    operand = current->data;
    if (operand[0] == '\0'){
        errorf(line_index,"Extra comma at the end of text");
        *error_found = TRUE;
    }


    current = get_list_head(operands_list);



    if (strcmp(instruction_token, ".data") == 0) {
        int counter = 0;
        while (current != NULL) {
            operand = current->data;
            remove_leading_and_trailing_whitespaces(operand, operand);

            printf("operand: %s\n",operand);

            if (!is_valid_integer(operand)) {
                errorf(line_index, "Invalid operand in .data instruction: '%s'", operand);
                *error_found = TRUE;
                free_list(&operands_list, free);
                free(line_copy);
                return;
            }

            (*DC)+=1;
            counter+=1;
            printf("counter value: %d\n", counter);
            current = current->next;
        }
        *IC+=counter;
    } else if (strcmp(instruction_token, ".string") == 0) {
        debugf(line_index,"found string");
        if (get_list_length(operands_list) != 1) {
            errorf(line_index, ".string instruction expects a single string operand");
            *error_found = TRUE;
            free_list(&operands_list, free);
            free(line_copy);
            return;
        }

        operand = operands_list->data;
        remove_leading_and_trailing_whitespaces(operand, operand);

        if (!is_valid_string(operand)) {
            errorf(line_index, "Invalid string in .string instruction: '%s'", operand);
            *error_found = TRUE;
            free_list(&operands_list, free);
            free(line_copy);
            return;
        }
        (*DC) += strlen(operand);
        (*IC) += strlen(operand) -1;

        debugf(line_index, "IC after string: %lu, DC after string: %lu",*IC,*DC);
    } else {
        errorf(line_index, "Unexpected instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }

    free_list(&operands_list, free);
    free(line_copy);
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
        unsigned long *IC,
        int *error_found
) {
    if (instruction == DATA || instruction == STRING) {
        if (strcmp(label,"")!=0) {
            add_symbol(symbol_table, label,*IC, *DC, DATA_PROPERTY, line_index);
        }
        parse_data_or_string_instruction(
                instruction_token,line_content, operands, label, symbol_table, DC,IC, error_found, line_index);
    } else if (instruction == EXTERN) {
        if (strcmp(label,"")!=0) {
            warnf(line_index, "Label '%s' cannot be associated with '.extern' instruction", label);
        }
        parse_extern_instruction(operands, symbol_table, error_found, line_index);
    } else if (instruction == ENTRY) {
        if (strcmp(label,"")!=0) {
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
    DoublyLinkedList *current_node = get_list_head(line_list);

    printf("starting first pass...\n");

    while (current_node != NULL) {
        Line *line_entry = (Line *)current_node->data;
        char *line_content = line_entry->data;
        char *label = line_entry->label;
        int index = line_entry->index;
        debugf(index, "line content: %s",line_content);
        debugf(index, "Current IC count: %lu, current DC count: %lu",IC,DC);



        if (
            !is_string_begin_with_substring(line_content, ";") && line_content[0] != '\0') {
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

   check_double_commas(line_index,line_copy,error_found);

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
                    line_content, instruction, label, line_index, token, tokens->next, symbol_table, DC,IC, error_found);
        } else {
            if (strchr(token,',')){
                errorf(line_index,"Illegal comma after command");
            } else {
                errorf(line_index, "Unrecognized command or instruction: '%s'", token);
            }
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
    debugf(line_index,"Found extern command");
    if (get_list_length(operands) != 1) {
        errorf(line_index, ".extern instruction expects exactly one operand");
        *error_found = TRUE;
        return;
    }

    operand = (char *)operands->data;
    add_symbol(symbol_table, operand, 0,0, EXTERNAL_PROPERTY, line_index);
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
