#ifndef ENCODED_LINE_H
#define ENCODED_LINE_H
#include "../utils/commons.h"
#include "../utils/logger.h"

/** Those definition are for ARE value according to the bit of each one in the assignment instructions */

#define A 4 /*100*/
#define R 2 /*010*/
#define E 1 /*001*/

/**Struct to represent a binary code of a line with additional fields
 * for immediate value and unresolved symbols with their indexed line for later use*/
typedef struct {
    unsigned int opcode : 6;         /* Bits 23-18 */
    unsigned int src_addressing : 2; /* Bits 17-16 */
    unsigned int src_reg : 3;        /* Bits 15-13 */
    unsigned int dst_addressing : 2; /* Bits 12-11 */
    unsigned int dst_reg : 3;        /* Bits 10-8 */
    unsigned int funct : 5;          /* Bits 7-3 */
    unsigned int are : 3;            /* Bits 2-0 */

    /** If immediate addressing, store the value */
    int immediate_value;

    /** if unresolved symbol store it to be resolved in second pass (along with its line in case of error)*/
    char *unresolved_symbol;
    int line_index;

} EncodedLine;

/**Creates new encoded line*/
EncodedLine* create_encoded_line();

/**Free encoded line*/
void free_encoded_line(EncodedLine* encoded_line);

/**Setters for encoded lines values*/

void encoded_line_set_opcode(EncodedLine* line, unsigned int opscode);
void encoded_line_set_funct(EncodedLine* line, unsigned int funct);
void encoded_line_set_src_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_dst_addressing(EncodedLine* line, unsigned int mode);
void encoded_line_set_reg(EncodedLine* line, unsigned int reg, int is_src);
void encoded_line_set_are(EncodedLine* line, unsigned int are_value);
void encoded_line_set_unresolved_symbol(EncodedLine* line, char* symbol);
void encoded_line_set_data(EncodedLine *encoded_line,int value, int are);
#endif
