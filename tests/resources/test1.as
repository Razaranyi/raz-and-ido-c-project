;11  12
MAIN:        mov r3, LIST
LOOP:        prn #-48
             lea STR, r6
             inc r6
             sub r1, r4
             bne END
             stop
STR:         .string "abcdef"
LIST:        .data 6, -9, 100, 12
K:           .data 31
             .extern END