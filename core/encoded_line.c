#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "encoded_line.h"


EncodedLine* create_encoded_line() {
    EncodedLine *line = (EncodedLine *)malloc(sizeof(EncodedLine));
    if (!line) {
        exit(1);
    }
    memset(line, 0, sizeof(EncodedLine));
    return line;
}
void free_encoded_line(EncodedLine *line) {
    if (!line) return;
    if (line->unresolved_symbol) {
        free(line->unresolved_symbol);
        line->unresolved_symbol = NULL;
    }
    free(line);
}


void encoded_line_set_opcode(EncodedLine* line, unsigned int opscode) {
    line->opcode = opscode;
}

void encoded_line_set_funct(EncodedLine* line,  unsigned int funct) {
    line->funct = funct;
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
void encoded_line_set_reg(EncodedLine* line, unsigned int reg, int is_dest){
    if (is_dest){
        encoded_line_set_dst_reg(line,reg);
    } else{
        encoded_line_set_src_reg(line,reg);
    }
}

void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol, int is_src) {
    if (line->unresolved_symbol) {
        free(line->unresolved_symbol);
    }
    line->unresolved_symbol = allocate_string(symbol);
    line->unresolved_is_src = is_src;
}

void encoded_line_set_immediate_with_are(EncodedLine *encoded_line, int value, unsigned int are) {
    /* Mask to ensure value is 21 bits (2's complement) */
    unsigned int masked_value = (unsigned int)(value & 0x1FFFFF); /* Mask lower 21 bits */

    /* Shift the masked value to fit bits 23-3 */
    masked_value <<= 3;

    /* Add ARE to the lower 3 bits */
    masked_value |= (are & 0x7); /* Ensure ARE is 3 bits */

    /* Store the combined value in the immediate_value field */
    encoded_line->immediate_value = masked_value;
    encoded_line->are = 4;
}

void encoded_line_set_data(EncodedLine *encoded_line,int value){
    /*do dummy for now*/
    encoded_line->immediate_value = value;
}

void print_encoded_line_binary(EncodedLine *line) {
    unsigned int combined_value = 0;
    int i = 0;

    /* Combine the fields into a single 24-bit integer */
    combined_value |= (line->opcode & 0x3F) << 18;         /* 6 bits for opcode (23-18) */
    combined_value |= (line->src_addressing & 0x3) << 16;  /* 2 bits for src addressing (17-16) */
    combined_value |= (line->src_reg & 0x7) << 13;         /* 3 bits for src reg (15-13) */
    combined_value |= (line->dst_addressing & 0x3) << 11;  /* 2 bits for dst addressing (12-11) */
    combined_value |= (line->dst_reg & 0x7) << 8;          /* 3 bits for dst reg (10-8) */
    combined_value |= (line->funct & 0x1F) << 3;           /* 5 bits for funct (7-3) */
    combined_value |= (line->are & 0x7);                   /* 3 bits for ARE (2-0) */

    printf("Binary representation: ");
    for (i = 23; i >= 0; i--) {
        printf("%d", (combined_value >> i) & 1);
        if (i % 4 == 0) {
            printf(" "); /* Add spacing every 4 bits for readability */
        }
    }
    printf("\n");
}

void print_encoded_line_values(EncodedLine *line){
    printf(
            "opcode: %d\naddress_src: %d\nreg_src: %d\naddress_dest: %d\nreg_dest: %d\n""funct: %d\nARE: %d\n",
            line->opcode,
            line->src_addressing,
            line->src_reg,
            line->dst_addressing,
            line->dst_reg,
            line->funct,
            line->are
            );

}

void print_encoded_immediate_with_are(EncodedLine *encoded_line) {
    unsigned int combined_value = (unsigned int)encoded_line->immediate_value;
    int i=0;

    printf("Binary representation of immediate + ARE: ");
    for (i = 22; i >= 0; i--) {
        printf("%d", (combined_value >> i) & 1);
        if (i % 4 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}
