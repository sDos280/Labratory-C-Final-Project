#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init("\"hello\"\"world\"\"\"");
    peek_string();
    peek_string();
    peek_string();
    print_token_list();

    lexer_free();

    return 0;
}