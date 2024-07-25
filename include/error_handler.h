#ifndef LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER
#define LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER

#include "string.h"
#include "token.h"

typedef enum {
    TokenErrorKind,
    CharErrorKind
} ErrorKind;

typedef enum {
    LexerErrorKind,
    PreprocessorErrorKind,
    ParserErrorKind,
    AstCheckerErrorKind,
    EmitterErrorKind
} ErrorCaller;

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
    ErrorCaller caller; /* the caller of the error */
    
    struct ErrorList *next;  /* the next error */
} ErrorList;


typedef struct ErrorHandler {
    String string;  /* the source file string the errors are in */
    char * filePath; /* the relative path to the file */
    ErrorList * errorList;  /* the error list */
} ErrorHandler;

/**
 * Initializes the error handler with the provided source string.
 * @param handler The error hadler.
 * @param string The source file string.
 * @param filePath The relative path to the string.
 */
void error_handler_init(ErrorHandler * handler, String string, char * filePath);

/**
 * Pushes a new token error to the end of the error handler's error list.
 * @param error The token error to be added.
 * @param caller The caller kind.
 */
void error_handler_push_token_error(ErrorHandler * handler, ErrorCaller caller, TokenError error);

/**
 * Pushes a new char error to the end of the error handler's error list.
 * @param error The char error to be added.
 * @param caller The caller kind.
 */
void error_handler_push_char_error(ErrorHandler * handler, ErrorCaller caller, CharError error);

/**
 * Flushes (outputs) the error handler's error list to the user.
 */
void error_handler_flush_error_list(ErrorHandler * handler);

/**
 * Frees the memory of the error handler's error list.
 */
void error_handler_free_error_list(ErrorHandler * handler);

#endif /* LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER */
