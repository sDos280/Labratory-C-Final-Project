funcTwoLocal: clr r5
              mov r1, r6
              add r2, *r3
              sub r4, r5
                  sub *r4, r5
funcTwo:
    inc r1



              mov r7, r6



              stop

labelOne:     .data 256, -512, 1024, -2048

stringThree:  .string "String inside the data section."
.extern funcTwo