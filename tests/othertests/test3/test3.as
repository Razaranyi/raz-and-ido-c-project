START:       clr r1
             jmp &MIDDLE
             stop
mcro I
             inc r1
             dec r1
mcroend
MIDDLE:      bne &END
             add r2, r3
END:         dec r2
             lea MISSING, r5
             .data 10, 20
             .entry START
             .entry NOT_DEFINED
             .extern MISSING
             .entry MISSING