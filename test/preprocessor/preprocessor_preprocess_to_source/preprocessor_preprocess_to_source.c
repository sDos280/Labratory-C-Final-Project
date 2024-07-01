#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    Preprocessor preprocessor;

    lexer_init_file(&lexer, "test1");
    lexer_lex(&lexer);
    error_handler_flush_error_list(&lexer.errorHandler);
        
    preprocessor_init(&preprocessor, lexer);
    preprocessor_generate_macro_list(&preprocessor, lexer.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);

    preprocessor_preprocess_to_source(&preprocessor, lexer.string);

    string_info(preprocessor.string);

    lexer_free(&lexer);
    preprocessor_free(&preprocessor);

    return 0;
}