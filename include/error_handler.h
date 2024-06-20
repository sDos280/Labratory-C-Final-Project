#ifndef LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER
#define LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER

#include "string.h"
#include "token.h"

typedef enum {
    LexerTokenErrorKind,
    LexerCharErrorKind,
    PreprocessorErrorKind,
} ErrorKind;

typedef struct TokenError {
    Token token;  /* the token the error talks about */
    String message;  /* the message from the caller */
} TokenError;

typedef struct CharError {
    char ch;  /* the char the error talks about */
    unsigned int index; /* the index of the char in the source file */
    unsigned int indexInLine; /* the line index of the char in the char's line */
    unsigned int line; /* the line of the char in the source file */
    String message;  /* the message from the caller */
} CharError;

typedef struct ErrorList {
    union {
        TokenError tokenError;  /* the current error */
        CharError charError;  /* the current error */
    } error;

    ErrorKind kind; /* the kind of the current error */
    
    struct LexerErrorList *next;  /* the next error */
} ErrorList;


typedef struct ErrorHandler {
    String source;  /* the source file string the errors are in */
    LexerErrorList * errorList;  /* the error list */
} ErrorHandler;

/**
 * Initializes the error handler with the provided source string.
 * @param handler The error hadler.
 * @param source The source file string.
 */
void error_handler_init(ErrorHandler * handler, String source);

/**
 * Pushes a new token error to the end of the error handler's error list.
 * @param error The token error to be added.
 */
void error_handler_push_token_error(ErrorHandler * handler, TokenError error);

/**
 * Pushes a new char error to the end of the error handler's error list.
 * @param error The char error to be added.
 */
void error_handler_push_char_error(ErrorHandler * handler, CharError error);

/**
 * Flushes (outputs) the error handler's error list to the user.
 */
void error_handler_flush_error_list(ErrorHandler * handler);

/**
 * Frees the memory of the error handler's error list.
 */
void error_handler_free_error_list(ErrorHandler * handler);

#endif /* LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER */
