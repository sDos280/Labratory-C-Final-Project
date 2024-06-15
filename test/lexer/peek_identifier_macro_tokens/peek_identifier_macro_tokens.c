#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, "macr\nendmacr\n");
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_identifier(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);

    return 0;
}