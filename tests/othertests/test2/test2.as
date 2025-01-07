MAIN:        mov r8, LIST
             add #5, 
             sub r1, r4, r3
             jmp MISSING_LABEL
             stop
mcro INVALID_MACRO
             add r1, r2
             stop
mcroend
STR:         .string "example
LIST:        .data 7, , , -10
INVALID_MACRO