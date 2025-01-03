MAIN:       add  r3, LIST
LOOP:       prn  #48
            lea  STR, r6
            inc  r6
            mov  r3, K
            sub  r1, r4
            bne  MID
mcro        my_macro
            cmp  K, #-6
            bne  &END
            dec  K
mcroend
            my_macro
            jmp  &LOOP
MID:        prn  K
            clr  r7
END:        stop
STR:        .string "abcdef"
LIST:       .data 6, -9, -100, 32767, -32768
K:          .data 31