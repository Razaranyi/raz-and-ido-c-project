#include <string.h>
#include "instruction.h"

Instruction get_instruction_enum(const char* instruction_name) {
    if (strcmp(instruction_name, ".data") == 0) return DATA;
    if (strcmp(instruction_name, ".string") == 0) return STRING;
    if (strcmp(instruction_name, ".entry") == 0) return ENTRY;
    if (strcmp(instruction_name, ".extern") == 0) return EXTERN;

    /* Not a recognized directive */
    return INVALID;
}


void parse_data_or_string_instruction(
        char *instruction_token,
        char *line_content,
        DoublyLinkedList *operands,
        char *label,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair,
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
    AddressEncodedPair *addressEncodedPair;
    EncodedLine *null_line;
    AddressEncodedPair *nullPair;


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
        int value = 0;
        while (current != NULL) {
            EncodedLine *encoded_line = create_encoded_line();

            operand = current->data;
            remove_leading_and_trailing_whitespaces(operand, operand);


            if (!assign_valid_integer(operand,&value)) {
                errorf(line_index, "Invalid operand in .data instruction: '%s'", operand);
                *error_found = TRUE;
                free_list(&operands_list, free);
                free(line_copy);
                return;
            }

            debugf(line_index,"assigning data instruction,address: %lu value: %d ",*IC,value);
            encoded_line_set_data(encoded_line,value, -1);
            addressEncodedPair = create_address_encoded_pair(*IC,encoded_line);
            add_to_list(address_encoded_line_pair,addressEncodedPair);
            (*DC)+=1;
            *IC+=1;
            current = current->next;
        }

    } else if (strcmp(instruction_token, ".string") == 0) {
        int i = 1;

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
        while (operand[i] != '\"'){
            EncodedLine* operand_encoded_line = create_encoded_line();
            debugf(line_index,"assigning string instruction to IC: %lu value: %d ",*IC,operand[i]);
            encoded_line_set_data(operand_encoded_line,operand[i], -1);
            addressEncodedPair = create_address_encoded_pair(*IC,operand_encoded_line);
            add_to_list(address_encoded_line_pair,addressEncodedPair);
            (*IC) += 1;
            i+=1;
        }
        /* Add the terminating '\0' character as a separate word */
        null_line = create_encoded_line();
        encoded_line_set_data(null_line, 0, -1); /* null terminator */
        nullPair = create_address_encoded_pair(*IC, null_line);
        add_to_list(address_encoded_line_pair, nullPair);

        (*IC) += 1;
        (*DC) += strlen(operand);


        debugf(line_index, "IC after string: %lu, DC after string: %lu",*IC,*DC);
    } else {
        errorf(line_index, "Unexpected instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }

    free_list(&operands_list, free);
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
    if (get_list_length_from_current(operands) != 1) {
        errorf(line_index, ".extern instruction expects exactly one operand");
        *error_found = TRUE;
        return;
    }

    operand = (char *)operands->data;
    add_symbol(symbol_table, operand, 0,0, EXTERNAL_PROPERTY, line_index);
}

void parse_entry_instruction(DoublyLinkedList *operands, DoublyLinkedList *entry_list, int *error_found, int line_index) {
    Entry *entry;
    char *operand = (char *)operands->data;
    if (get_list_length_from_current(operands) != 1) {
        errorf(line_index, ".entry instruction expects exactly one operand");
        *error_found = TRUE;
        return;
    }

    if (!is_string_equal_by_regex(operand, "^[a-zA-Z][a-zA-Z0-9]*$")) {
        errorf(line_index, "Invalid label name '%s' in .entry instruction", operand);
        *error_found = TRUE;
        return;
    }
    entry = allocate_entry_mem(operand,line_index);

    debugf(line_index,"Adding entry %s line %d to entry list",entry->name,entry->index);

    add_to_list(entry_list,entry);
}


void process_instruction_line(
        char *line_content,
        Instruction instruction,
        char *label,
        int line_index,
        char *instruction_token,
        DoublyLinkedList *operands,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair,
        DoublyLinkedList *entry_list,
        unsigned long *DC,
        unsigned long *IC,
        int *error_found
) {
    if (instruction == DATA || instruction == STRING) {
        if (strcmp(label,"")!=0) {
            add_symbol(symbol_table, label,*IC, *DC, DATA_PROPERTY, line_index);
        }
        parse_data_or_string_instruction(
                instruction_token,
                line_content,
                operands,
                label,
                symbol_table,
                address_encoded_line_pair,
                DC,
                IC,
                error_found,
                line_index);
    } else if (instruction == EXTERN) {
        if (strcmp(label,"")!=0) {
            warnf(line_index, "Label '%s' cannot be associated with '.extern' instruction", label);
        }
        parse_extern_instruction(operands, symbol_table, error_found, line_index);
    } else if (instruction == ENTRY) {
        if (strcmp(label,"")!=0) {
            warnf(line_index, "Label '%s' cannot be associated with '.entry' instruction", label);
        }
        parse_entry_instruction(operands,entry_list,error_found,line_index);
    } else {
        errorf(line_index, "Unknown instruction: '%s'", instruction_token);
        *error_found = TRUE;
    }
}



