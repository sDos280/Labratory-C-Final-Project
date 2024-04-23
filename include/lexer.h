#ifndef LABRATORY_C_FINAL_PROJECT_LEXER_H
#define LABRATORY_C_FINAL_PROJECT_LEXER_H

#include "string_util.h"
#include "token.h"

typedef struct Lexer{
    String string;      /* the input string */
    unsigned int index;          /* the current index */
    unsigned int line_index; /* the index of the current char in current line (the \n will be the last char in the current line)*/
    char currentChar;   /* the current char looked at in the lexer */
    unsigned int currentLine; /* the current line looked at in the lexer (we start counting from 0)*/
    TokenList * tokens;   /* the output token list */
}Lexer;

typedef enum {
    LexerTokenErrorKind,
    LexerCharErrorKind,
} LexerErrorKind;

typedef struct LexerTokenError{
    Token token;  /* the token the error talks about */
    String message;  /* the message from the caller */
}LexerTokenError;

typedef struct LexerCharError{
    char ch;  /* the char the error talks about */
    unsigned int index; /* the index of the char in the source file*/
    unsigned int line_index; /* the line index of the char in the source file */
    String message;  /* the message from the caller */
}LexerCharError;

typedef struct LexerErrorList{
    union
    {
        LexerTokenError tokenError;  /* the current error */
        LexerCharError charError;  /* the current error */
    };

    LexerErrorKind kind;
    
    struct LexerErrorList * next;  /* the next error */
}LexerErrorList;

void lexer_init(char * sourceString); /* init lexer function */
void lexer_free(); /* reset lexer, after this function call you must not the lexer before calling lexer_init  */
void print_token_list(); /* print the token list in a formated way */
void peek_comment();  /* peek a comment, the caller must use the function when the token really appears in the current contex */
void peek_separator();  /* peek a separator, the caller must use the function when the token really appears in the current contex */
void peek_number();  /* peek a number, the caller must use the function when the token really appears in the current contex */
void peek_string(); /* peek a primery string expresion (AKA "..."), the caller must use the function when the token really appears in the current contex */
void peek_non_op_instruction(); /* peek a none operative instructions (AKA .data/.string/...), the caller must use the function when the token really appears in the current contex */
void peek_identifier(); /* peek an identifier (if an identifier has a meaning of an operative instruction\regirstor\macro token it would be classefied as that) the caller must use the function when the token really appears in the current contex*/

void push_lexer_token_error(LexerTokenError error); /* push a new lexer token error to the end of the lexer's error list*/
void flush_lexer_error_list(); /* flush (output) the lexer's error list to the user */
#endif /*LABRATORY_C_FINAL_PROJECT_LEXER_H*/
