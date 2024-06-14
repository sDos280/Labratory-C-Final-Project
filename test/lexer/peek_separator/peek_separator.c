#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("*,:##**,,");
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_peek_separator();
    lexer_print_token_list();

    lexer_free();

    return 0;
}