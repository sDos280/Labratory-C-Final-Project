#ifndef LABRATORY_C_FINAL_PROJECT_LEXER_H
#define LABRATORY_C_FINAL_PROJECT_LEXER_H

#include "string_util.h"

typedef enum{
    COMMENT,    /* / ; ... */
    REGISTER,   /* r0, r1, r2, r3, r4, r5, r6, r7 */
    IDENTIFIER,
    NUMBER,     /* 25 -25 +25 */
    STRING,      /* */
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
    JSR,        /* jst */
    RTS,        /* rts */
    STOP,       /* STOP */
    MACR,       /* macr */
    ENDMACR,    /* endmacr */
    DATA_OP,    /* .data */
    STRING_OP,  /* .string */
    ENTRY_OP,   /* .entry */
    EXTERN_OP,  /* .extern */

    COMMA,      /* , */
    COLON,      /* : */
    HASHTAG,    /* # */
    STAR,       /* * */

    IN_MACRO,    /* a token that represents the inside data of a macro */

    EOL,        /* end of line */
    EOT,        /* end of token stream */
}TokenKind;

typedef struct Token{
    TokenKind kind;  /* the token kind */
    int start;  /* the start char index of the token in the file */
    String string; /* the string data */
}Token;

typedef struct TokenList{
    Token token;  /* the current token */
    struct TokenList * next;  /* the next token */
}TokenList;

typedef struct Lexer{
    String string;      /* the input string */
    int index;          /* the current index */
    char currentChar;   /* the current char looked at in the lexer */
    TokenList * tokens;   /* the output token list */
}Lexer;

void lexer_init(char * sourceString); /* init lexer function */
void print_token_list(); /* print the token list in a formated way */
void peek_comment();  /* peek a comment, the caller must use the function when the token really appears in the current contex */
void peek_separator();  /* peek a separator, the caller must use the function when the token really appears in the current contex */

#endif /*LABRATORY_C_FINAL_PROJECT_LEXER_H*/
