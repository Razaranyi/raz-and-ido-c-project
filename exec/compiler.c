#include "compiler.h"
/*global var for final address*/
unsigned long final_IC = 0;
unsigned long final_DC = 0;


void check_double_commas(int line_index, char *line_copy, int *error_found) {
    int last_was_comma = 0;
    int i;

    for (i = 0; line_copy[i] != '\0'; i++) {
        char current_char = line_copy[i];

        if (current_char == ',') {
            if (last_was_comma) {
                errorf(line_index, "Multiple consecutive commas: '%s'",line_copy);
                *error_found = TRUE;
                return;
            }
            last_was_comma = 1;
        } else if (!isspace(current_char)) {
            last_was_comma = 0;
        }
    }
}

void process_line(
        char *line_content,
        char *label,
        int line_index,
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair,
        DoublyLinkedList *entry_list,
        unsigned long *IC,
        unsigned long *DC,
        int *error_found
) {
    DoublyLinkedList *tokens = NULL;
    char *line_copy = allocate_string(line_content);
    char *token;
    infof(line_index,"start processing line: %s",line_copy);

   check_double_commas(line_index,line_copy,error_found);

    split_string_by_separator(line_copy, " ", &tokens, -1);
    token = (char *)tokens->data;
    cut_spaces_start(token);


    debugf(line_index, "checking token: %s",token);

    if (find_command(token) != NULL) {
        debugf(line_index,"command found");
        process_command_line(line_content,
                             label,
                             line_index,
                             token,
                             symbol_table,
                             address_encoded_line_pair,
                             IC,
                             error_found);
    } else {
        Instruction instruction = get_instruction_enum(token);

        if (instruction != INVALID) {
           debugf(line_index,"instruction found");

            process_instruction_line(
                    line_content,
                    instruction,
                    label,
                    line_index,
                    token,
                    tokens->next,
                    symbol_table,
                    address_encoded_line_pair,
                    entry_list,
                    DC,
                    IC,
                    error_found);
        } else {
            if (strchr(token,',')){
                errorf(line_index,"Illegal comma after command '%s'", token);
            } else {
                errorf(line_index, "Unrecognized command or instruction: '%s'", token);
            }
            *error_found = TRUE;
        }
    }

    free_list(&tokens, free);
    free(line_copy);
}

int first_pass(DoublyLinkedList *line_list,
               DoublyLinkedList *symbol_table,
               DoublyLinkedList *address_encoded_line_pair,
               DoublyLinkedList *entry_list) {
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
            process_line(line_content, label, index, symbol_table,address_encoded_line_pair,entry_list, &IC, &DC, &error_found);
        }

        current_node = current_node->next;
    }
    debugf(-1, "IC: %lu, DC: %lu",IC,DC);
    final_DC = DC -1;
    final_IC = IC-100 - final_DC;


    return !error_found;
}



void resolve_symbol(
        DoublyLinkedList *symbol_table,
        DoublyLinkedList *address_encoded_line_pair_list,
        int *error_found
) {
    DoublyLinkedList *current = get_list_head(address_encoded_line_pair_list);
    while (current != NULL) {
        AddressEncodedPair *pair = (AddressEncodedPair *)current->data;
        EncodedLine *line = pair->encoded_line;

        if (line->unresolved_symbol != NULL) {
            Symbol *symbol;
            if (!symbols_table_get_symbol(symbol_table, line->unresolved_symbol, &symbol)) {
                errorf(-1, "Unresolved symbol '%s' not found", line->unresolved_symbol);
                *error_found = TRUE;
            } else {
                unsigned long symbol_address = symbol->address;


                if (line->are == R) {
                    if (is_property(*symbol,EXTERNAL_PROPERTY)){
                        add_external_usage(symbol,(long )pair->address);
                        encoded_line_set_data(line,symbol_address,E);
                    } else
                        encoded_line_set_data(line,symbol_address,R);

                } else if (line->are == 4) {
                    unsigned long dist = (unsigned long)symbol_address - (unsigned long)(pair->address) +1;
                    debugf(-1, "Calc %lu - %lu = %lu",(unsigned long)symbol_address,(unsigned long)pair->address,dist);
                    encoded_line_set_data(line, dist, A);
                }

                free(line->unresolved_symbol);
                line->unresolved_symbol = NULL;
            }
        }

        current = current->next;
    }
}


int second_pass(DoublyLinkedList *symbol_table,
                DoublyLinkedList *address_encoded_line_pair_list,
                DoublyLinkedList *entry_list) {
    int error_found = FALSE;

    /* Mark all entries */
    mark_entries(entry_list, symbol_table, &error_found);

    /* Resolve all symbol references */
    resolve_symbol(symbol_table, address_encoded_line_pair_list, &error_found);

    return !error_found;
}

