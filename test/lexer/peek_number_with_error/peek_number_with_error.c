#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("-556\n252\n+6546\n+\n-");
    peek_number();
    peek_next_line();
    peek_number();
    peek_next_line();
    peek_number();
    peek_next_line();
    peek_number();
    peek_next_line();
    peek_number();
    print_token_list();
    flush_lexer_error_list();
    
    free_lexer_error_list();
    lexer_free();

    return 0;
}