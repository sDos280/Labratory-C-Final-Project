#include "../include/ast_checker.h"
#include "../include/string_util.h"

#define MULTIPLIER (37)
#define HIGHST_INTEGER_IN_COMPILER  (signed int)(((2 << (14-1))) -1)
#define LOWEST_INTEGER_IN_COMPILER (signed int)(-(2 << (14-1)))

unsigned long hash(String str) {
    unsigned long h;
    unsigned const char *us;

    /* cast s to unsigned const char * */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) str.data;

    h = 0;
    while(*us != '\0') {
        h = h * MULTIPLIER + *us;
        us++;
    } 

    return h;
}

void ast_checker_init(AstChecker * astChecker, Lexer lexer){
    error_handler_init(&astChecker->errorHandler, lexer.string, lexer.filePath);
}

void ast_checker_free(AstChecker * astChecker){
    error_handler_free_error_list(&astChecker->errorHandler);
}

void ast_checker_check_data_guidance_sentence(AstChecker * astChecker, DataNode node){
    TokenRefrenceList * numbers = node.numbers;
    TokenError error;
    int value;

    while (numbers != NULL){
        value = atoi(numbers->token->string.data);
        if (value > HIGHST_INTEGER_IN_COMPILER){
            error.message = string_init_with_data("This number is too high");
            error.token = *numbers->token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerKind, error);
        }else if (value < LOWEST_INTEGER_IN_COMPILER){
            error.message = string_init_with_data("This number is too low");
            error.token = *numbers->token;

            error_handler_push_token_error(&astChecker->errorHandler, AstCheckerKind, error);
        }

        numbers = numbers->next;
    }
}