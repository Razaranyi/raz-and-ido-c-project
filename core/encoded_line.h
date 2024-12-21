#ifndef ENCODED_LINE_H
#define ENCODED_LINE_H
#include "../utils/commons.h"
#include "../utils/logger.h"

#define A 4
#define R 2
#define E 1

typedef struct {
    unsigned int opcode : 6;         /* Bits 23-18 */
    unsigned int src_addressing : 2; /* Bits 17-16 */
    unsigned int src_reg : 3;        /* Bits 15-13 */
    unsigned int dst_addressing : 2; /* Bits 12-11 */
    unsigned int dst_reg : 3;        /* Bits 10-8 */
    unsigned int funct : 5;          /* Bits 7-3 */
    unsigned int are : 3;            /* Bits 2-0 */

    /* If immediate addressing, store the value */
    int immediate_value;

    char *unresolved_symbol;
} EncodedLine;

EncodedLine* create_encoded_line();
void free_encoded_line(EncodedLine* encoded_line);

void encoded_line_set_opcode(EncodedLine* line, unsigned int opscode);
void encoded_line_set_funct(EncodedLine* line, unsigned int funct);
void encoded_line_set_src_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_dst_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_reg(EncodedLine* line, unsigned int reg, int is_src);
void encoded_line_set_are(EncodedLine* line, unsigned int are_value);
void encoded_line_set_immediate(EncodedLine* line, int value);
void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol);
void print_encoded_line_binary(EncodedLine *line);
void print_encoded_line_values(EncodedLine *line);
void encoded_line_set_immediate_with_are(EncodedLine *encoded_line, int value, unsigned int are);
void print_encoded_immediate_with_are(EncodedLine *encoded_line);
void encoded_line_set_data(EncodedLine *encoded_line,int value, int are);
#endif
