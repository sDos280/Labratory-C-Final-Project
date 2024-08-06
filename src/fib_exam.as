; file fibonacci.ps

.entry MAIN
.entry FIBONACCI
.extern PRINT

MAIN:   mov #10, r0       ; Number of Fibonacci terms to calculate
        jsr FIBONACCI     ; Jump to the Fibonacci function
        stop              ; Stop execution
        prn r4            ; Print the value of the Fibonacci number
        lea PRINT, r4      ; Just a test of extern


FIBONACCI:
        clr r1            ; r1 = 0 (first Fibonacci number)
        mov #1, r2        ; r2 = 1 (second Fibonacci number)
        clr r3            ; r3 will be used to count terms
        clr r4            ; r4 will hold the current Fibonacci number

        jmp FIBLOOP      ; Jump to the FIBLOOP to start the loop

FIBLOOP:
        cmp r3, r0        ; Compare the count with the number of terms
        bne ENDFIB       ; If count >= number of terms, exit loop
        prn r1            ; Print the current Fibonacci number
        add r1, r2        ; r2 = r1 + r2 (next Fibonacci number)
        mov r2, r4        ; Store the result in r4
        mov r4, r2        ; Update r2 to the new Fibonacci number
        sub r1, r2        ; Update r2 to the old Fibonacci number
        add #1, r3        ; Increment the term count
        jmp FIBLOOP      ; Repeat the loop

ENDFIB:
        rts               ; Return from the function
