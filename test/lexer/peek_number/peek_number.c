#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

void main(){
    lexer_init("-556\n252\n+6546");
    peek_number();
    peek_next_line();
    peek_number();
    peek_next_line();
    peek_number();
    print_token_list();

    lexer_free();
}