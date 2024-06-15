#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    Lexer lexer;
    lexer_init(&lexer, "\"hello\"\"world\"\"\"\"");
    lexer_peek_string(&lexer);
    lexer_peek_string(&lexer);
    lexer_peek_string(&lexer);
    lexer_peek_string(&lexer);
    lexer_print_token_list(&lexer);
    lexer_flush_lexer_error_list(&lexer);

    lexer_free_lexer_error_list(&lexer);
    lexer_free(&lexer);

    return 0;
}