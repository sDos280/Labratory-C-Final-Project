#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("x: .data 23\nmain: dec x\ninc *r1\n move *r1, *r2");
    lexer_lex();
    lexer_print_token_list();

    lexer_free();

    return 0;
}