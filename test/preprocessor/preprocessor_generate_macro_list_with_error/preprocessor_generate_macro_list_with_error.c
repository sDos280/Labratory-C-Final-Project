#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/preprocessor.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    Preprocessor preprocessor;
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr hello   \nmain: dec x\ninc *r1\n  endmacr  \n");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);
    
    preprocessor_init(&preprocessor, lexer);
    preprocessor_generate_macro_list(&preprocessor, lexer.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);
    
    preprocessor_print_macro_list(&preprocessor);

    lexer_free(&lexer);
    preprocessor_free(&preprocessor);
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");

    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr   \nmain: dec x\ninc *r1\n  endmacr  \n");
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
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");
    
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr  hello main: dec x\ninc *r1\n  endmacr  \n");
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
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");
    
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr hello  \nmain: dec x\ninc *r1  endmacr  \n");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);
    error_handler_flush_error_list(&lexer.errorHandler);
    
    preprocessor_init(&preprocessor, lexer);
    preprocessor_generate_macro_list(&preprocessor, lexer.string);
    error_handler_flush_error_list(&preprocessor.errorHandler);
    
    preprocessor_print_macro_list(&preprocessor);

    
    error_handler_free_error_list(&preprocessor.errorHandler);
    lexer_free(&lexer);
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");
    
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr gedfd  \nmain: dec x\ninc *r1\n  endmacr  \n");
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
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");
    
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr gedfd  \nmain: dec x\ninc *r1\n  endmacr ");
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
    /* ------------------------------------------------------------------ */

    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr gedfd  \nmain: dec x\ninc *r1\n  endmacr \n macr gedfd  \nninc ffdf fdf *r1\n  endmacr");
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
    /* ------------------------------------------------------------------ */

    printf("\n\n\n");
    
    /* ------------------------------------------------------------------ */
    lexer_init_char_pointer(&lexer, "macr gedfd  \nmain: dec x\ninc *r1\n  \n");
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
    /* ------------------------------------------------------------------ */

    return 0;
}