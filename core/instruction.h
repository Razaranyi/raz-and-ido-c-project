#ifndef INSTRUCTION_H
#define INSTRUCTION_H

typedef enum {
    INVALID = 0,
    DATA = 1,
    STRING = 2,
    ENTRY = 3,
    EXTERN = 4
} Instruction;

/* Return enum of instruction // 0 when not an instruction */
Instruction get_instruction_enum(const char* instruction_name);

#endif
