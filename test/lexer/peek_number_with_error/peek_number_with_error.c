#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("-556\n252\n+6546\n+\n-");
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_print_token_list();
    lexer_flush_lexer_error_list();
    
    lexer_free_lexer_error_list();
    lexer_free();

    return 0;
}