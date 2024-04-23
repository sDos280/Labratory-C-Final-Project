#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("macr\nendmacr\n");
    peek_identifier();
    peek_next_line();
    peek_identifier();
    peek_next_line();
    print_token_list();

    lexer_free();

    return 0;
}