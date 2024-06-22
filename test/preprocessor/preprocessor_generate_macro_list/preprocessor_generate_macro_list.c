#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    Preprocessor preprocessor;
    lexer_init(&lexer, "macr hello   \nmain: dec x\ninc *r1\n  endmacr   \t\n           macr hello2   \n      x: .data 23\nendmacr");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);
    error_handler_flush_error_list(&lexer.errorHandler);
    
    preprocessor_init(&preprocessor, lexer);
    preprocessor_generate_macro_list(&preprocessor, lexer.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);
    
    preprocessor_print_macro_list(&preprocessor);

    error_handler_free_error_list(&lexer.errorHandler);
    error_handler_free_error_list(&preprocessor.errorHandler);
    lexer_free(&lexer);

    return 0;
}