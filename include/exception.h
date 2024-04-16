#ifndef LABRATORY_C_FINAL_PROJECT_EXCEPTION_H
#define LABRATORY_C_FINAL_PROJECT_EXCEPTION_H

#include "string_util.h"
#include "token.h"

typedef enum {
    LexerExceptionKind,
} ExceptionKind;

typedef struct LexerException{
    Token token;  /* the token the exception talks about */
    String message;  /* the message from the caller */
}LexerException;

typedef struct Exception{
    LexerException kind;  /* the kind of the exeption */
    union
    {
        LexerException lexerException;
    };
    
}Exception;

typedef struct ExceptionList{
    Exception exception;  /* the current exception */
    struct ExceptionList * next;  /* the next exception */
}ExceptionList;

void append_exception(Exception exception); /* append an exception to the end of the exception list*/
#endif /*LABRATORY_C_FINAL_PROJECT_EXCEPTION_H*/
