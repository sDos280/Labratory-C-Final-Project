#ifndef LABRATORY_C_FINAL_PROJECT_TOKEN_H
#define LABRATORY_C_FINAL_PROJECT_TOKEN_H

#include "string_util.h"

typedef enum{
    COMMENT,    /* ; ... */
    REGISTER,   /* r0, r1, r2, r3, r4, r5, r6, r7 */
    IDENTIFIER,
    NUMBER,     /* 25 -25 +25 */
    STRING,      /* "..." */
    MOV,        /* mov */
    CMP,        /* cmp */
    ADD,        /* add */
    SUB,        /* sub */
    LEA,        /* lea */
    CLR,        /* clr */
    NOT,        /* not */
    INC,        /* inc */
    DEC,        /* dec */
    JMP,        /* jmp */
    BNE,        /* bne */
    RED,        /* red */
    PRN,        /* prn */
    JSR,        /* jsr */
    RTS,        /* rts */
    STOP,       /* stop */
    MACR,       /* macr */
    ENDMACR,    /* endmacr */
    DATA_INS,    /* .data */
    STRING_INS,  /* .string */
    ENTRY_INS,   /* .entry */
    EXTERN_INS,  /* .extern */

    COMMA,      /* , */
    COLON,      /* : */
    HASHTAG,    /* # */
    STAR,       /* * */

    /*IN_MACRO,     a token that represents the inside data of a macro */
    
    ErrorToken, /* an error has raised in this token */ 

    EOL,        /* end of line */
    EOFT        /* end of file token (end of token stream) \0 */
}TokenKind;

typedef struct Token{
    TokenKind kind;  /* the token kind */
    unsigned int index;  /* the index of the starter char of the token (in the file) */
    unsigned int indexInLine; /* the index of the starter char the token (in the token line)*/
    unsigned int line;  /* the line the token is inside */
    String string; /* the string data */
}Token;

typedef struct TokenList{
    Token token;  /* the current token */
    struct TokenList * next;  /* the next token */
}TokenList;

typedef struct TokenListRefrence {
    Token * token; /* the current token */
    struct TokenRefrenceList * next; /* the next token */
}TokenListRefrence;

#endif /*LABRATORY_C_FINAL_PROJECT_TOKEN_H*/
