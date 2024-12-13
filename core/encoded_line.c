#include <stdlib.h>
#include <string.h>
#include "encoded_line.h"


EncodedLine* create_encoded_line() {
    EncodedLine *line = (EncodedLine *)malloc(sizeof(EncodedLine));
    if (!line) {
        exit(1);
    }
    memset(line, 0, sizeof(EncodedLine));
    return line;
}

void free_encoded_line(EncodedLine* encoded_line) {
    if (encoded_line->unresolved_symbol) {
        free(encoded_line->unresolved_symbol);
    }
    free(encoded_line);
}

void encoded_line_set_opcode(EncodedLine* line, Command command) {
    line->opcode = (unsigned int )command.opcode;
}

void encoded_line_set_funct(EncodedLine* line, Command command) {
    line->funct = (unsigned int)command.funct;
}

void encoded_line_set_src_addressing(EncodedLine* line, unsigned int mode) {
    line->src_addressing = mode;
}

void encoded_line_set_dst_addressing(EncodedLine* line, unsigned int mode) {
    line->dst_addressing = mode;
}

void encoded_line_set_src_reg(EncodedLine* line, unsigned int reg) {
    line->src_reg = reg;
}

void encoded_line_set_dst_reg(EncodedLine* line, unsigned int reg) {
    line->dst_reg = reg;
}

void encoded_line_set_are(EncodedLine* line, unsigned int are_value) {
    line->are = are_value;
}

void encoded_line_set_immediate(EncodedLine* line, int value) {
    line->immediate_value = value;
}

void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol, int is_src) {
    if (line->unresolved_symbol) {
        free(line->unresolved_symbol);
    }
    line->unresolved_symbol = allocate_string(symbol);
    line->unresolved_is_src = is_src;
}
