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

#endif /* LABRATORY_C_FINAL_PROJECT_ERROR_HANDLER */
