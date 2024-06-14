#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("\"hello\"\"world\"\"\"");
    lexer_peek_string();
    lexer_peek_string();
    lexer_peek_string();
    lexer_print_token_list();

    lexer_free();

    return 0;
}