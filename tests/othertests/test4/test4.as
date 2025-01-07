;     28  17
.extern EXTERNLABEL
.extern EXTERNLABEL2
MAIN:        mov r1, MYLABEL
             add r3, r2
             sub #15, r4
             lea STR, r6
             USEMACRO
             clr r1
             jmp &LOOP

mcro USEMACRO
             inc r3
             prn r3
mcroend

mcro CALLHELPER
             dec r4
             add r5, r2
mcroend

LOOP:        bne &END
             inc r5
             PRINTANDJUMP
             stop

mcro PRINTANDJUMP
             prn r6
             jmp &START
mcroend
STR:         .string "test string"
MYLABEL:     .data 1, 2, 3, 4, 5
END:         add r1, r3
             lea EXTERNLABEL, r6
             .entry MAIN
START:       clr r0
             lea MYLABEL, r1
             add r2, r4
             bne EXTERNLABEL
