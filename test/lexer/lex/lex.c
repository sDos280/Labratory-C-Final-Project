#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, "x: .data 23\nmain: dec x\ninc *r1\n move *r1, *r2");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);

    return 0;
}