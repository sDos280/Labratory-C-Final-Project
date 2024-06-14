#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("; adasdfasdf asdf \n; hello world \n\n\n");
    lexer_peek_comment();
    lexer_peek_next_line();
    lexer_peek_comment();
    lexer_peek_next_line();
    lexer_peek_next_line();
    lexer_peek_next_line();
    lexer_print_token_list();

    lexer_free();

    return 0;
}