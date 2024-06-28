#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init_char_pointer(&lexer, "*,:##**,,");
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_separator(&lexer);
    lexer_print_token_list(&lexer);
    
    lexer_free(&lexer);

    return 0;
}