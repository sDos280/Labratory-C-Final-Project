#ifndef LABRATORY_C_FINAL_PROJECT_LEXER_H
#define LABRATORY_C_FINAL_PROJECT_LEXER_H

#include "string_util.h"
#include "token.h"

typedef struct Lexer {
    String string;      /* the input string */
    unsigned int index;          /* the current index */
    unsigned int indexInLine; /* the index of the current char in current line (the \n will be the last char in the current line)*/
    char ch;   /* the current char looked at in the lexer */
    unsigned int line; /* the current line looked at in the lexer (we start counting from 1)*/
    
    char * filePath;
    
    TokenList * tokens;   /* the output token list */
} Lexer;

typedef enum {
    LexerTokenErrorKind,
    LexerCharErrorKind
} LexerErrorKind;

typedef struct LexerTokenError {
    Token token;  /* the token the error talks about */
    String message;  /* the message from the caller */
} LexerTokenError;

typedef struct LexerCharError {
    char ch;  /* the char the error talks about */
    unsigned int index; /* the index of the char in the source file */
    unsigned int indexInLine; /* the line index of the char in the char's line */
    unsigned int line; /* the line of the char in the source file */
    String message;  /* the message from the caller */
} LexerCharError;

typedef struct LexerErrorList {
    union {
        LexerTokenError tokenError;  /* the current error */
        LexerCharError charError;  /* the current error */
    } error;

    LexerErrorKind kind; /* the kind of the current error */
    
    struct LexerErrorList *next;  /* the next error */
} LexerErrorList;

/**
 * Initializes the lexer with the provided source string.
 * @param sourceString The source string to be lexed.
 */
void lexer_init(char *sourceString);

/**
 * Frees the resources used by the lexer.
 * After calling this function, the lexer must not be used before calling lexer_init again.
 */
void lexer_free(void);

/**
 * Prints the token list in a formatted way.
 */
void print_token_list(void);

/**
 * Peeks a comment token.
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_comment(void);

/**
 * Peeks a newline character token (e.g., \n).
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_next_line(void);

/**
 * Peeks a separator token.
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_separator(void);

/**
 * Peeks a number token.
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_number(void);

/**
 * Peeks a primary string expression token (e.g., "...").
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_string(void);

/**
 * Peeks a non-operative instruction token (e.g., .data, .string, ...).
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_non_op_instruction(void);

/**
 * Peeks an identifier token.
 * @note If an identifier has a meaning of an operative instruction, register, or macro token, it will be classified as such.
 * @note The caller must use this function when the token appears in the current context.
 */
void peek_identifier(void);

/**
 * Performs a full lexer pass on the source code.
 */
void lex(void);

/**
 * Pushes a new lexer token error to the end of the lexer's error list.
 * @param error The lexer token error to be added.
 */
void push_lexer_token_error(LexerTokenError error);

/**
 * Pushes a new lexer char error to the end of the lexer's error list.
 * @param error The lexer char error to be added.
 */
void push_lexer_char_error(LexerCharError error);

/**
 * Flushes (outputs) the lexer's error list to the user.
 */
void flush_lexer_error_list(void);

/**
 * Frees the memory of the error list.
 */
void free_lexer_error_list(void);

#endif /* LABRATORY_C_FINAL_PROJECT_LEXER_H */
