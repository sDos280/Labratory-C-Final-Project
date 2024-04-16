#include "exception.h"

static ExceptionList exceptionList;

void append_exception(Exception exception){
    ExceptionList * toAdd = malloc(sizeof(ExceptionList));
    toAdd->exception = exception;
    toAdd->next = NULL;

    ExceptionList * last = &exceptionList;
    while (last->next != NULL){
        last = last->next;
    }

    last->next = toAdd;
}