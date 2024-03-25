//
// Created by dor283 on 3/25/24.
//

#ifndef LABRATORY_C_FINAL_PROJECT_LEXER_H
#define LABRATORY_C_FINAL_PROJECT_LEXER_H

typedef enum{
    COMMENT,    // ; ... \n
    REGISTER,   // r0, r1, r2, r3, r4, r5, r6, r7
    IDENTIFIER,
    NUMBER,     // 25 -25 +25
    STRING      //
    MOV,        // mov
    CMP,        // cmp
    ADD,        // add
    SUB,        // sub
    LEA,        // lea
    CLR,        // clr
    NOT,        // not
    INC,        // inc
    DEC,        // dec
    JMP,        // jmp
    BNE,        // bne
    RED,        // red
    PRN,        // prn
    JSR,        // jst
    RTS,        // rts
    STOP,       // STOP
    MACR,       // macr
    ENDMACR,    // endmacr
    COMMA,      // ,
    DATA_OP,    // .data
    STRING_OP,  // .string
    ENTRY_OP,   // .entry
    EXTERN_OP,  // .extern

    COLON,      // :
    HASHTAG,    // #
    STAR,       // *

    EOT,        // end of token stream
}TokenKind;

typedef struct {
    TokenKind kind;  // the token kind
    int start;  // the start char index of the token in the file
    char * string; // the token string (null terminated)
}Token;

typedef struct {
    Token token;  // the current token
    TokenList * next;  // the next token
}TokenList;

#endif //LABRATORY_C_FINAL_PROJECT_LEXER_H
