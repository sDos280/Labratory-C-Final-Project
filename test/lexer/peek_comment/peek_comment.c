#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("; adasdfasdf asdf \n; hello world \n\n\n");
    peek_comment();
    peek_next_line();
    peek_comment();
    peek_next_line();
    peek_next_line();
    peek_next_line();
    print_token_list();

    lexer_free();

    return 0;
}