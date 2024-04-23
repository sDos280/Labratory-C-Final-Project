#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("\n\n\n\n");
    peek_next_line();
    peek_next_line();
    peek_next_line();
    peek_next_line();
    print_token_list();

    lexer_free();

    return 0;
}