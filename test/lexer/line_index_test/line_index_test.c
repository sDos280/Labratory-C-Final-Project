#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("-556,252\n+6546-55:666666\n99");
    peek_number();
    peek_separator();
    peek_number();
    peek_next_line();
    peek_number();
    peek_number();
    peek_separator();
    peek_number();
    peek_next_line();
    peek_number();
    print_token_list();

    lexer_free();

    return 0;
}