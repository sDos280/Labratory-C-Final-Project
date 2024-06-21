#ifndef LABRATORY_C_FINAL_PROJECT_LEXER_H
#define LABRATORY_C_FINAL_PROJECT_LEXER_H

#include "char_util.h"
#include "string_util.h"
#include "token.h"

typedef struct Lexer {
    String string;      /* the input string */
    unsigned int index;          /* the current index */
    unsigned int indexInLine; /* the index of the current char in current line (the \n will be the last char in the current line)*/
    char ch;   /* the current char looked at in the lexer */
    unsigned int line; /* the current line looked at in the lexer (we start counting from 1)*/
    
    char * filePath; /* the relative path to the file */

    TokenList * tokens;   /* the output token list */
} Lexer;

/**
 * Initializes the lexer with the provided source string.
 * @param sourceString The source string to be lexed.
 */
void lexer_init(Lexer *lexer, char *sourceString);

/**
 * Frees the resources used by the lexer.
 * After calling this function, the lexer must not be used before calling lexer_init again.
 */
void lexer_free(Lexer *lexer);

/**
 * Prints the token list in a formatted way.
 */
void lexer_print_token_list(Lexer *lexer);

/**
 * Advance the lexer char pointer to the next char.
 * Update the lexer internal variables du to that advancement
 */
void lexer_peek_char(Lexer * lexer);

/**
 * Peeks a comment token.
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_comment(Lexer *lexer);

/**
 * Peeks a newline character token (e.g., \n).
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_next_line(Lexer *lexer);

/**
 * Peeks a separator token.
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_separator(Lexer *lexer);

/**
 * Peeks a number token.
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_number(Lexer *lexer);

/**
 * Peeks a primary string expression token (e.g., "...").
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_string(Lexer *lexer);

/**
 * Peeks a non-operative instruction token (e.g., .data, .string, ...).
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_non_op_instruction(Lexer *lexer);

/**
 * Peeks an identifier token.
 * @note If an identifier has a meaning of an operative instruction, register, or macro token, it will be classified as such.
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_identifier(Lexer *lexer);

/**
 * Peeks an end of file token.
 * @note The caller must use this function when the token appears in the current context.
 */
void lexer_peek_end_of_file(Lexer *lexer);

/**
 * Performs a full lexer pass on the source code.
 */
void lexer_lex(Lexer *lexer);

#endif /* LABRATORY_C_FINAL_PROJECT_LEXER_H */
