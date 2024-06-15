#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, "-556,252\n+6546-55:666666\n99");
    lexer_peek_number(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_separator(&lexer);
    lexer_peek_number(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_number(&lexer);
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);

    return 0;
}