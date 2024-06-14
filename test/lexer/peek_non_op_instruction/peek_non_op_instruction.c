#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init(".string.data.entry.string");
    lexer_peek_non_op_instruction();
    lexer_peek_non_op_instruction();
    lexer_peek_non_op_instruction();
    lexer_peek_non_op_instruction();
    lexer_print_token_list();

    lexer_free();
    
    return 0;
}