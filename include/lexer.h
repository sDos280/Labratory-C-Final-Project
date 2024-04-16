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

void lexer_init(char * sourceString); /* init lexer function */
void lexer_free(); /* reset lexer, after this function call you must not the lexer before calling lexer_init  */
void print_token_list(); /* print the token list in a formated way */
void peek_comment();  /* peek a comment, the caller must use the function when the token really appears in the current contex */
void peek_separator();  /* peek a separator, the caller must use the function when the token really appears in the current contex */
void peek_number();  /* peek a number, the caller must use the function when the token really appears in the current contex */
void peek_string(); /* peek a primery string expresion (AKA "..."), the caller must use the function when the token really appears in the current contex */
void peek_non_op_instruction(); /* peek a none operative instructions (AKA .data/.string/...), the caller must use the function when the token really appears in the current contex */
#endif /*LABRATORY_C_FINAL_PROJECT_LEXER_H*/
