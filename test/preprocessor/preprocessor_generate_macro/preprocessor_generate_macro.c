#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    Preprocessor preprocessor;
    lexer_init(&lexer, "macr hello   \nmain: dec x\ninc *r1\n  endmacr   \t\n");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);
    error_handler_flush_error_list(&lexer.errorHandler);
    
    preprocessor_init(&preprocessor, lexer);
    preprocessor_generate_macro(&preprocessor, lexer.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);
    
    printf("macro expansion identifier: %s\n", preprocessor.macroList->macro.identifier.string.data);
    printf("macro expansion start:\n%s\n", preprocessor.macroList->macro.expansion.data);
    printf("macro expansion end!\n");

    error_handler_free_error_list(&lexer.errorHandler);
    error_handler_free_error_list(&preprocessor.errorHandler);
    lexer_free(&lexer);

    return 0;
}