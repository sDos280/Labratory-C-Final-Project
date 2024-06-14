#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("x: .data 23\nmain: dec x\ninc *r1\n mov [*r1, *r2]\n.dataError \"fdsfasdf\"\n");
    lexer_lex();
    lexer_print_token_list();
    lexer_flush_lexer_error_list();
    
    lexer_free_lexer_error_list();
    lexer_free();

    return 0;
}