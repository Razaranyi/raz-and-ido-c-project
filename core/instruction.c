#include <string.h>
#include "instruction.h"

Instruction get_instruction_enum(const char* instruction_name) {
    if (strcmp(instruction_name, ".data") == 0) return DATA;
    if (strcmp(instruction_name, ".string") == 0) return STRING;
    if (strcmp(instruction_name, ".entry") == 0) return ENTRY;
    if (strcmp(instruction_name, ".extern") == 0) return EXTERN;

    /* Not a recognized directive */
    return INVALID;
}
