#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/string_util.h"

int main(){
    int i;
    Lexer lexer;
    Preprocessor preprocessor;

    char * tests[8] = {
        "test1", 
        "test2", 
        "test3", 
        "test4", 
        "test5", 
        "test6", 
        "test7", 
        "test8"
    };

    for (i = 0; i < 8; i++){
        lexer_init_file(&lexer, tests[i]);
        lexer_lex(&lexer);
        lexer_print_token_list(&lexer);
        error_handler_flush_error_list(&lexer.errorHandler);
        
        preprocessor_init(&preprocessor, lexer);
        preprocessor_generate_macro_list(&preprocessor, lexer.string);
        error_handler_flush_error_list(&preprocessor.errorHandler);
        
        preprocessor_print_macro_list(&preprocessor);

        lexer_free(&lexer);
        preprocessor_free(&preprocessor);
    }

    return 0;
}