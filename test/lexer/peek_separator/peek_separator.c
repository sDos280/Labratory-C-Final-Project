#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

void main(){
    lexer_init("*,:##**,,");
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    peek_separator();
    print_token_list();

    lexer_free();
}