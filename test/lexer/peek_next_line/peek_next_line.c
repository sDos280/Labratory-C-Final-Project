#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init_char_pointer(&lexer, "\n\n\n\n");
    lexer_peek_next_line(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_peek_next_line(&lexer);
    lexer_print_token_list(&lexer);

    lexer_free(&lexer);

    return 0;
}