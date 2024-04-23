#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
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

    return 0;
}