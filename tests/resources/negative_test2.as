.extern EXTERNLABEL
MAIN:        mov r1, MYLABEL
             add r3, r9
             sub #15      
             lea NOT_DEFINED_LABEL, r6 
             jmp &LOOP
             stop

LOOP:        bne &END
             add r2       
             inc r5
             prn r6
             jmp &NOT_DEFINED_LABEL 
             stop

STR:         .string "example
MYLABEL:     .data 7, -1, , 3 
END:         clr r0
             lea MYLABEL, r1
             bne EXTERNLABEL
