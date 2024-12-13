#ifndef ENCODED_LINE_H
#define ENCODED_LINE_H

typedef struct {
    unsigned int opcode : 4;
    unsigned int funct : 4;
    unsigned int src_addressing : 2;
    unsigned int dst_addressing : 2;
    unsigned int src_reg : 3;
    unsigned int dst_reg : 3;
    unsigned int are : 3;

    /* If immediate addressing, store the value */
    int immediate_value;

    char *unresolved_symbol;
    int unresolved_is_src;

} EncodedLine;

EncodedLine* create_encoded_line();
void free_encoded_line(EncodedLine* encoded_line);

void encoded_line_set_opcode(EncodedLine* line, unsigned int opcode);
void encoded_line_set_funct(EncodedLine* line, unsigned int funct);
void encoded_line_set_src_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_dst_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_src_reg(EncodedLine* line, unsigned int reg);
void encoded_line_set_dst_reg(EncodedLine* line, unsigned int reg);
void encoded_line_set_are(EncodedLine* line, unsigned int are_value);
void encoded_line_set_immediate(EncodedLine* line, int value);
void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol, int is_src);

#endif
