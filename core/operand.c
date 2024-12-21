
#include "operand.h"


int is_valid_label_name(char *label);
int is_register(char *operand_str);

/* Allocates memory for an Operand struct and initializes its fields */
Operand* allocate_operand() {
    Operand *operand = (Operand *)malloc(sizeof(Operand));
    if (!operand) {
        fprintf(stderr, "Memory allocation error for Operand\n");
        exit(1);
    }
    memset(operand, 0, sizeof(Operand));
    return operand;
}

/* Frees memory allocated for an Operand struct */
void free_operand(Operand *operand) {
    if (operand == NULL){
        return;
    }
    if (operand->operand_str) {
        free(operand->operand_str);
    }
    if (operand->symbol_name) {
        free(operand->symbol_name);
    }

}

/* Extracts the register index from a register operand (e.g., 'r3' -> 3) */
int get_register_index(char *raw_op) {
    if (strlen(raw_op) != 2 || raw_op[0] != 'r' || !isdigit(raw_op[1])) {
        return -1;
    }
    return raw_op[1] - '0';
}

/* Determines the addressing mode of the operand */
int determine_addressing_mode(char *operand_str) {
    if (operand_str[0] == '#') {
        return IMMEDIATE_ADDRESSING;
    } else if (operand_str[0] == '&') {
        return RELATIVE_ADDRESSING;
    } else if (is_register(operand_str)) {
        return REGISTER_ADDRESSING;
    } else if (is_valid_label_name(operand_str)) {
        return DIRECT_ADDRESSING;
    } else if (strstr(operand_str, ".") != NULL) {
        return DIRECT_ADDRESSING; /* Indexed addressing, treated as direct for simplicity */
    } else {
        return -1; /* Invalid addressing mode */
    }
}

/* Parses an operand string and fills the Operand struct */
int parse_operand(char *operand_str, int index, Operand *operand, int line_index)  {
    operand->index = index;
    operand->operand_str = allocate_string(operand_str);
    operand->addressing_mode = determine_addressing_mode(operand_str);


    if (operand->addressing_mode == -1) {
        errorf(line_index, "Invalid addressing mode for operand '%s", operand_str, line_index);
        return FALSE;
    }

    switch (operand->addressing_mode) {
        case IMMEDIATE_ADDRESSING:
            operand->is_immediate = TRUE;
            operand->immediate_value = atoi(operand_str + 1); /* Skip '#' */
            break;
        case DIRECT_ADDRESSING:
            operand->is_symbol = TRUE;
            operand->symbol_name = allocate_string(operand_str);
            break;
        case RELATIVE_ADDRESSING:
            operand->is_symbol = TRUE;
            operand->symbol_name = allocate_string(operand_str + 1); /* Skip '&' */
            break;
        case REGISTER_ADDRESSING:
            operand->is_register = TRUE;
            operand->register_number = get_register_index(operand_str);
            if (operand->register_number < 0 || operand->register_number > 7) {
                errorf(line_index, "Invalid register '%s'", operand_str);
                return FALSE;
            }
            break;
        default:
            errorf(line_index, "Unknown addressing mode for operand '%s'", operand_str);
            return FALSE;
    }

    return TRUE;
}



/* Counts the extra words needed for a set of operands */
int count_extra_addresses_words(Operand operands[], int operand_count, DoublyLinkedList *address_encoded_line_pair, unsigned long IC,int line_index) {
    int extra_words = 0;
    int both_registers = FALSE;
    int current_address;
    int i;




    if (operand_count == 2 &&
        operands[0].addressing_mode == REGISTER_ADDRESSING &&
        operands[1].addressing_mode == REGISTER_ADDRESSING) {
        /* Both operands are registers; they don't need a word*/
        both_registers = TRUE;
    }
    if (!both_registers){
        for (i = 0; i < operand_count; i++) {
            AddressEncodedPair *address_encoded_pair;
            int is_reg;
            EncodedLine *encodedLine = create_encoded_line();

            switch (operands[i].addressing_mode) {
                case IMMEDIATE_ADDRESSING:
                    extra_words +=1;
                    encoded_line_set_data(encodedLine, operands[i].immediate_value, A);
                    is_reg = FALSE;
                    break;

                case DIRECT_ADDRESSING:
                    extra_words+=1;
                    encoded_line_set_are(encodedLine, R);
                    debugf(line_index,"Adding %s as unresolved symbol",operands[i].symbol_name);
                    encoded_line_set_unresolved_symbol(encodedLine,operands[i].symbol_name);
                    encodedLine->line_index = line_index;
                    is_reg = FALSE;
                    break;

                case RELATIVE_ADDRESSING:
                    extra_words += 1;
                    encoded_line_set_are(encodedLine, A);
                    debugf(line_index,"Adding %s as unresolved symbol",operands[i].symbol_name);
                    encoded_line_set_unresolved_symbol(encodedLine,operands[i].symbol_name);
                    encodedLine->line_index = line_index;
                    is_reg = FALSE;
                    break;

                case REGISTER_ADDRESSING:
                    is_reg = TRUE;
                    break;

                default:
                    break;
            }
            if(!is_reg){
                current_address = IC + extra_words;
                address_encoded_pair = create_address_encoded_pair(current_address, encodedLine);
                add_to_list(address_encoded_line_pair, address_encoded_pair);
            } else{
                free_encoded_line(encodedLine);
            }
        }
    }


    return extra_words;
}

/* Helper function to check if a string is a valid register */
int is_register(char *operand_str) {
    int reg_num = get_register_index(operand_str);
    return reg_num >= 0 && reg_num <= 7;
}

/* Helper function to validate label names */
int is_valid_label_name(char *label) {
    int i;
    if (!isalpha(label[0])) {
        return FALSE;
    }
    if (strlen(label) > MAX_LABEL_LENGTH) {
        return FALSE;
    }
    for (i = 1; label[i] != '\0'; i++) {
        if (!isalnum(label[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

/* Validates if a given string is a valid integer */
int is_valid_integer(char *operand) {
    char *p;
    if (operand == NULL || *operand == '\0') {
        return FALSE;
    }
    p = operand;
    if (*p == '#'){
        p++;
    }
    if (*p == '+' || *p == '-') {
        p++;
    }
    while (*p) {
        if (!isdigit(*p)) {
            return FALSE;
        }
        p++;
    }
    return TRUE;
}

int assign_valid_integer(char *operand, int *out_value) {
    char *p;
    int sign = 1;
    long result = 0; /* Use a long to avoid overflow issues, then cast */

    if (operand == NULL || *operand == '\0') {
        return FALSE;
    }

    p = operand;

    /* If operand starts with '#', skip it */
    if (*p == '#') {
        p++;
    }

    /* Check for optional sign */
    if (*p == '+') {
        p++;
    } else if (*p == '-') {
        sign = -1;
        p++;
    }

    /* Ensure at least one digit exists */
    if (!isdigit(*p)) {
        return FALSE;
    }

    while (*p) {
        if (!isdigit(*p)) {
            return FALSE;
        }

        /* Convert digit to integer and accumulate */
        result = result * 10 + (*p - '0');
        p++;
    }

    /* Apply sign */
    result = result * sign;

    /* If out_value is not NULL, store the result */
    if (out_value != NULL) {
        *out_value = (int)result;
    }

    return TRUE;
}


/* Validates if a given string is a valid string literal */
int is_valid_string(char *operand) {
    size_t len = strlen(operand);
    size_t i;
    if (len < 2 || operand[0] != '"' || operand[len - 1] != '"') {
        return FALSE;
    }
    for ( i = 1; i < len - 1; i++) {
        if (!isprint(operand[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

int contains_whitespace(char *operand){
    while (*operand) {
        if (isspace((unsigned char)*operand)) {
            return TRUE;
        }
        operand++;
    }
    return FALSE;
}


/* Checks if an operand is valid */
int is_valid_operand(char *operand) {
    return (is_valid_integer(operand) || is_valid_string(operand) || isalpha(*operand) || operand[0] == '&') && !contains_whitespace(operand) ;
}
