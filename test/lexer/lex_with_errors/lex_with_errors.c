#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init_char_pointer(&lexer, "x: .data 23\nmain: dec x\ninc *r1\n mov [*r1, *r2]\n.dataError \"fdsfasdf\"\n");
    lexer_lex(&lexer);
    lexer_print_token_list(&lexer);
    lexer_flush_lexer_error_list(&lexer);
    
    lexer_free_lexer_error_list(&lexer);
    lexer_free(&lexer);

    return 0;
}