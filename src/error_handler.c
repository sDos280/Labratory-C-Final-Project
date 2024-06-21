#include "../include/error_handler.h"

void error_handler_init(ErrorHandler * handler, String source){
    handler->source = source;
    handler->errorList = NULL;
}

void error_handler_push_token_error(ErrorHandler * handler, ErrorCaller caller, TokenError error){
    ErrorList * toAdd = malloc(sizeof(ErrorList));
    toAdd->error.tokenError = error;
    toAdd->kind = TokenErrorKind;
    toAdd->caller = caller;
    toAdd->next = NULL;

    if (handler->errorList == NULL){
        /* we haven't added an error yet */
        handler->errorList = toAdd;
    }else{
        ErrorList * last = handler->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void error_handler_push_char_error(ErrorHandler * handler, ErrorCaller caller, CharError error){
    ErrorList * toAdd = malloc(sizeof(ErrorList));
    toAdd->error.charError = error;
    toAdd->kind = CharErrorKind;
    toAdd->caller = caller;
    toAdd->next = NULL;

    if (handler->errorList == NULL){
        /* we haven't added an error yet */
        handler->errorList = toAdd;
    }else{
        ErrorList * last = handler->errorList;
        while (last->next != NULL){
            last = last->next;
        }
        last->next = toAdd;
    }
}

void error_handler_flush_error_list(ErrorHandler * handler);

void error_handler_free_error_list(ErrorHandler * handler);

