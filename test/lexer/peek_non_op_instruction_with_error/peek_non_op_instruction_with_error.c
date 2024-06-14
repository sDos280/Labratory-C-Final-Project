#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init(".string\n.dataError\n.entry\n.string\n");
    lexer_peek_non_op_instruction();
    lexer_peek_next_line();
    lexer_peek_non_op_instruction();
    lexer_peek_next_line();
    lexer_peek_non_op_instruction();
    lexer_peek_next_line();
    lexer_peek_non_op_instruction();
    lexer_peek_next_line();
    lexer_print_token_list();
    lexer_flush_lexer_error_list();

    lexer_free();

    return 0;
}