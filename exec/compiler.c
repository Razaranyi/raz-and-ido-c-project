#include "compiler.h"

/* Remove the label from the line */
char* remove_label_from_line(char *line) {
    char *colon_pos = strchr(line, ':');
    if (colon_pos != NULL) {
        colon_pos++; /* Skip ':' */
        while (isspace((unsigned char)*colon_pos)) {
            colon_pos++; /* Skip whitespace */
        }
        return colon_pos;
    }
    return line;
}

/* Extract and validate a label */
char* extract_and_validate_label(
        char *line,
        DoublyLinkedList *symbol_table,
        int *error_found,
        int line_number
) {
    char label_name[30];
    char *colon_pos = strchr(line, ':');
    int label_length;

    if (colon_pos == NULL) {
        errorf(line_number, "Syntax error: Missing ':' after label");
        *error_found = TRUE;
        return NULL;
    }

    label_length = colon_pos - line;
    if (label_length >= 30) {
        errorf(line_number, "Label name too long");
        *error_found = TRUE;
        return NULL;
    }

    strncpy(label_name, line, label_length);
    label_name[label_length] = '\0';

    /* Validate label */
    if (is_command_name(label_name)) {
        errorf(line_number, "Label name '%s' conflicts with a command name", label_name);
        *error_found = TRUE;
        return NULL;
    }
    if (check_if_instruction(label_name)) {
        errorf(line_number, "Label name '%s' conflicts with an instruction name", label_name);
        *error_found = TRUE;
        return NULL;
    }

    if (check_if_registar(label_name)){
        errorf(line_number, "Label name '%s' conflicts with a register name", label_name);
        *error_found = TRUE;
        return NULL;
    }

    /* Allocate and return validated label name */
    return allocate_string(label_name);
}

/* Main first pass function */
int first_pass(DoublyLinkedList *lines_list, DoublyLinkedList *symbol_table) {
    int IC = 100;
    int DC = 0;
    int error_found = FALSE;

    DoublyLinkedList *current_node = get_list_head(lines_list);

    while (current_node != NULL) {
        Line *line = (Line *)current_node->data;
        char *line_content = line->data;
        int line_number = line->index;

        /* Skip empty or comment lines */
        if (!is_string_begin_with_substring(line_content, "\0") &&
            !is_string_begin_with_substring(line_content, ";")) {

            process_line(line_content, line_number, symbol_table, &IC, &DC, &error_found);
        }

        current_node = current_node->next;
    }

    return !error_found; /* Return TRUE if no errors were found */
}

void process_line(
        char *line_content,
        int line_number,
        DoublyLinkedList *symbol_table,
        int *IC,
        int *DC,
        int *error_found
) {
    char *label = NULL;
    char *token;
    DoublyLinkedList *tokens = NULL;
    char *line_copy = allocate_string(line_content);

    if (is_label(line_copy)) { /*TODO use label field in list*/
        label = extract_and_validate_label(line_copy, symbol_table, error_found, line_number);
        if (label == NULL) {
            /* Error in label processing; skip the line */
            free(line_copy);
            return;
        }

        /* Remove the label from the line */
        line_copy = remove_label_from_line(line_copy);
    }

    /* Tokenize the line */
    split_string_by_separator(line_copy, " \t", &tokens, -1);
    if (is_list_empty(tokens)) {
        free_list(&tokens, free);
        if (label != NULL) free(label);
        free(line_copy);
        return;
    }

    /* Get the first token (instruction or command) */
    token = (char *)tokens->data;

    /* Process based on line type */
    if (find_command(token) != NULL) {
        process_instruction_line(token, tokens->next, label, symbol_table, IC, error_found, line_number);
    } else {
        Instruction instruction = get_instruction_enum(token);
        if (instruction != INVALID) {
            process_command_line(token, tokens->next, label, symbol_table, DC, error_found, line_number);
        } else {
            errorf(line_number, "Unrecognized instruction or directive: '%s'", token);
            *error_found = TRUE;
            if (label != NULL) free(label);
        }
    }

    /* Free resources */
    free_list(&tokens, free);
    free(line_copy);
}


