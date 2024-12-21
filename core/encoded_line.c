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


void encoded_line_set_reg(EncodedLine* line, unsigned int reg, int is_dest){
    if (is_dest){
        encoded_line_set_dst_reg(line,reg);
    } else{
        encoded_line_set_src_reg(line,reg);
    }
}

void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol) {
    if (line->unresolved_symbol) {
        free(line->unresolved_symbol);
    }
    line->unresolved_symbol = allocate_string(symbol);
}


void encoded_line_set_data(EncodedLine *encoded_line,int value, int are){
    int padding = 0;
    if (are == -1) /*case data line*/
    {
        encoded_line->are = value & 0x7; /*extract bits 0-2*/
        padding = 3;
    }
    else /*other cases*/
        encoded_line->are = are & 0x7;
    encoded_line->opcode = (value >> (15 + padding)) & 0x3F; /*extract bits 18-23*/
    encoded_line->src_addressing = (value >> (13 + padding)) & 0x3; /*extract bits 16-17*/
    encoded_line->src_reg = (value >> (10 + padding)) & 0x7; /*extract bits 13-15*/
    encoded_line->dst_addressing = (value >> (8 + padding)) & 0x3; /*extract bits 11-12*/
    encoded_line->dst_reg = (value >> (5 + padding)) & 0x7; /*extract bits 8-10*/
    encoded_line->funct = (value >> padding) & 0x1F; /*extract bits 3-7*/
    if (are == -1) /*case data line*/
        encoded_line->are = value & 0x7; /*extract bits 0-2*/
    else /*other cases*/
        encoded_line->are = are & 0x7;
}



