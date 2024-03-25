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
    DATA_OP,    // .data
    STRING_OP,  // .string
    ENTRY_OP,   // .entry
    EXTERN_OP,  // .extern

    COMMA,      // ,
    COLON,      // :
    HASHTAG,    // #
    STAR,       // *

    IN_MACRO    // a token that represents the inside data of a macro

    EOL,        //end of line
    EOT,        // end of token stream
}TokenKind;

typedef struct Token{
    TokenKind kind;  // the token kind
    int start;  // the start char index of the token in the file
    unsigned char * string; // the token string (null terminated)
}Token;

typedef struct TokenList{
    Token token;  // the current token
    TokenList * next;  // the next token
}TokenList;

typedef struct Lexer{
    unsigned char * string;      // the input string
    int index;          // the current index
    char currentChar;   // the current char looked at in the lexer
    TokenList * tokens;   // the output token list
}Lexer;

void lexer_init(unsigned char * sourceString);
#endif //LABRATORY_C_FINAL_PROJECT_LEXER_H
