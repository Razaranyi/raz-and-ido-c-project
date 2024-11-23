

void first_pass(FILE *source_file, DoublyLinkedList *symbol_table, int *IC, int *DC, int *error_found) {
    char line[LEN_LINE];
    char clean_line[LEN_LINE];
    int line_counter = 0;

    while (fgets(line, sizeof(line), source_file) != NULL) {
        line_counter++;

        if (!clean_line[0] == '\0' && !clean_line[0] == ';') {
            process_line(clean_line, line_counter, symbol_table, IC, DC, error_found);
        }
    }
}

void process_line(char *line, int line_counter, DoublyLinkedList *symbol_table, int *IC, int *DC, int *error_found) {
    char *label = NULL;
    char line_copy[LEN_LINE];
    char *line_after_label = line;

    if (is_label(line)) {
        /* Extract and validate the label */
        label = extract_and_validate_label(line, line_counter, symbol_table, error_found);
        if (label == NULL) {
            // Error in label processing; skip line
            return;
        }

        // Remove the label from the line
        line_after_label = remove_label_from_line(line);
    }

    // Copy the line after label for tokenization
    strcpy(line_copy, line_after_label);

    // Tokenize to get the first word (instruction or directive)
    char *token = strtok(line_copy, " \t\n");
    if (token == NULL) {
        // Line is empty after removing label; skip
        if (label != NULL) free(label);
        return;
    }

    // Determine line type and process accordingly
    if (is_instruction(token)) {
        process_instruction_line(token, line_after_label, label, symbol_table, IC, error_found, line_counter);
    } else if (is_directive(token)) {
        process_command_line(token, line_after_label, label, symbol_table, DC, error_found, line_counter);
    } else {
        errorf(line_counter, "Unrecognized line type: '%s'", token);
        *error_found = TRUE;
        if (label != NULL) free(label);
    }
}
