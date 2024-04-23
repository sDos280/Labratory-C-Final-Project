#include "stdint.h"
#include "../../../include/lexer.h"
#include "../../../include/string_util.h"

int main(){
    lexer_init(".string\n.dataError\n.entry\n.string\n");
    peek_non_op_instruction();
    peek_next_line();
    peek_non_op_instruction();
    peek_next_line();
    peek_non_op_instruction();
    peek_next_line();
    peek_non_op_instruction();
    peek_next_line();
    print_token_list();
    flush_lexer_error_list();

    lexer_free();

    return 0;
}