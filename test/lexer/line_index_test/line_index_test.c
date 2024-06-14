#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("-556,252\n+6546-55:666666\n99");
    lexer_peek_number();
    lexer_peek_separator();
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_peek_number();
    lexer_peek_separator();
    lexer_peek_number();
    lexer_peek_next_line();
    lexer_peek_number();
    lexer_print_token_list();

    lexer_free();

    return 0;
}