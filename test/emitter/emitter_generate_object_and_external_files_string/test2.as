; file ps.as

.entry LIST
.entry MAIN
.extern L3
.entry K
.entry LOOP
.entry END
.entry STR
.extern fn1

STR:  .string "abcd"

MAIN: add r3, LIST
      jsr fn1

LIST: .data 6, -9
      .data -100
      
LOOP: prn #48
      lea STR, r6
      inc r6
      mov *r6, L3
      sub r1, r4
      cmp r3, #-6
      bne END
      add r7, *r6
      clr K
      sub L3, L3
      jmp LOOP
END:  stop


K:    .data 31
